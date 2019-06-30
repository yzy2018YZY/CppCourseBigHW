//1850059 ��1�� ��־Զ

#include "90-b5.h"

enum Direction
{
	Up = 0,
	Down,
	Left,
	Right
};

static void draw_all_blanks(const CONSOLE_GRAPHICS_INFO *const pCGI, const int map[][ColNum]);
static bool is_plane_body(int n);
static bool row_col_in_map(const int row, const int col);
static bool map_full(const int map[][ColNum]);
static bool check_full_compare(int num);
static bool row_col_in_map_head(const int row, const int col);
static int body_value_of_plane(const int map[][ColNum], const int row, const int col, const int direction);
static void set_value_of_plane(int map[][ColNum], const int row, const int col, const int direction, const int value);
static void update_top_status(const CONSOLE_GRAPHICS_INFO *const pCGI, const int score, const string &spack);
static bool send_plane_coordinates_with_direction(cmd_tcp_socket &client, const int row, const int col, const int direction);
static int find_planes(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client, string &spack, int map[][ColNum], int &score);
static int manual_mode_game_left_button(const CONSOLE_GRAPHICS_INFO *const pCGI, int map[][ColNum], int &hitted, const string &spack, const int row, const int col);
static int manual_mode_game_right_button(const CONSOLE_GRAPHICS_INFO *const pCGI, int map[][ColNum], int &hitted, const string &spack, const int headRow, const int headCol, const int row, const int col);
static int manual_mode_game(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client);
static int auto_mode_complete_one_plane(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client, string &spack, int map[][ColNum], int &score, const int row, const int col);
static int auto_mode_game_one_step(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client, string &spack, int map[][ColNum], int &score, const int row, const int col);
static void auto_mode_find_heads(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client, string &spack, int map[][ColNum], int &score, int &hitted);
static int auto_mode_game(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client);
static void game_prepare(CONSOLE_GRAPHICS_INFO *const pCGI);

//�������������е�ɫ��
static void draw_all_blanks(const CONSOLE_GRAPHICS_INFO *const pCGI, const int map[][ColNum])
{
	draw_points(pCGI, map, RowNum, ColNum, bdi);
}

//�ж�ĳ��ֵ��û�п����ǻ���auto_mode_complete_one_plane����ר��
static bool is_plane_body(int n)
{
	if (n == BDI_VALUE_BODY || n == BDI_VALUE_BLANK)
		return true;
	else
		return false;
}

//�ж�������Ƿ��ڵ�ͼ��
static bool row_col_in_map(const int row, const int col)
{
	return in_map(RowNum, ColNum, row, col);
}

//�ж�������Ƿ��ڻ�ͷ���ܴ��ڵķ�Χ��
static bool row_col_in_map_head(const int row, const int col)
{
	if (row_col_in_map(row, col) == false)
		return false;
	if (row < 2 && col < 2)
		return false;
	if (row < 2 && col >= ColNum - 2)
		return false;
	if (row >= RowNum - 2 && col < 2)
		return false;
	if (row >= RowNum - 2 && col >= ColNum - 2)
		return false;
	else
		return true;
}

//����ĳ����ͷ��ĳ�������ϵĻ����Ȩ�أ�����зǷ������򷵻�-1�����򷵻�0~20��ֵ
static int body_value_of_plane(const int map[][ColNum], const int row, const int col, const int direction)
{
	if (map[row][col] != BDI_VALUE_BLANK && map[row][col] != BDI_VALUE_HEAD)
		return -1;
	int ret = 0;
	if (direction == Down || direction == Up)
	{
		const int offsetX[] = { 0, -1, -2, 1, 2, 0, -1, 0, 1 }, offsetY[] = { 1, 1, 1, 1, 1, 2, 3, 3 ,3 };
		int dir = (direction == Down ? 1 : -1);
		for (int i = 0; i < sizeof(offsetX) / sizeof(offsetX[0]); i++)
		{
			if (map[row + offsetY[i] * dir][col + offsetX[i]] == BDI_VALUE_BODY)
				ret += (4 - offsetY[i]);
			else if (map[row + offsetY[i] * dir][col + offsetX[i]] != BDI_VALUE_BLANK)
				return -1;
		}
	}
	if (direction == Left || direction == Right)
	{
		const int offsetY[] = { 0, -1, -2, 1, 2, 0, -1, 0, 1 }, offsetX[] = { 1, 1, 1, 1, 1, 2, 3, 3 ,3 };
		int dir = (direction == Right ? 1 : -1);
		for (int i = 0; i < sizeof(offsetX) / sizeof(offsetX[0]); i++)
		{
			if (map[row + offsetY[i]][col + offsetX[i] * dir] == BDI_VALUE_BODY)
				ret += (4 - offsetX[i]);
			else if (map[row + offsetY[i]][col + offsetX[i] * dir] != BDI_VALUE_BLANK)
				return -1;
		}
	}
	return ret;
}

//������ĳ�������ϵ������ɻ��������ϼ�¼��ֵ
static void set_value_of_plane(int map[][ColNum], const int row, const int col, const int direction, const int value)
{
	if (direction == Down || direction == Up)
	{
		const int offsetX[] = { 0, -1, -2, 1, 2, 0, -1, 0, 1 }, offsetY[] = { 1, 1, 1, 1, 1, 2, 3, 3 ,3 };
		int dir = (direction == Down ? 1 : -1);
		for (int i = 0; i < sizeof(offsetX) / sizeof(offsetX[0]); i++)
		{
			map[row + offsetY[i] * dir][col + offsetX[i]] = value;
		}
		map[row][col] = value;
	}
	if (direction == Left || direction == Right)
	{
		const int offsetY[] = { 0, -1, -2, 1, 2, 0, -1, 0, 1 }, offsetX[] = { 1, 1, 1, 1, 1, 2, 3, 3 ,3 };
		int dir = (direction == Right ? 1 : -1);
		for (int i = 0; i < sizeof(offsetX) / sizeof(offsetX[0]); i++)
		{
			map[row + offsetY[i]][col + offsetX[i] * dir] = value;
		}
		map[row][col] = value;
	}
}

//�����������ĳ�������ϵĻ�ͷ��β˫�������Ϣ������false��ʾ������Ҫ��û�з��ͣ�����true��ʾ���ͳɹ�
static bool send_plane_coordinates_with_direction(cmd_tcp_socket &client, const int row, const int col, const int direction)
{
	const int X[] = { 0, 0, -3, 3 };
	const int Y[] = { -3, 3, 0, 0 };
	if (row_col_in_map(row + Y[direction], col + X[direction]) == false)
		return false;
	client.send_plane_coordinates(char(row + 'A'), col, char(row + Y[direction] + 'A'), col + X[direction]);
	return true;
}

//������������Ԥ�зɻ�λ�ò�����˫���꣬���سɹ�����ķɻ�����
static int find_planes(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client, string &spack, int map[][ColNum], int &score)
{
	bool blocked[RowNum][ColNum][4] = { false };
	int ret = 0;
	for (int i = 0; i < sizeof(CoordinateRowList) / sizeof(CoordinateRowList[0]); i++)
	{
		if (row_col_in_map_head(CoordinateRowList[i], CoordinateColList[i]) == true && (map[CoordinateRowList[i]][CoordinateColList[i]] == BDI_VALUE_BLANK))
		{
			for (int dir = 0; dir < 4; dir++)
			{
				if (blocked[CoordinateRowList[i]][CoordinateColList[i]][dir] == false && body_value_of_plane(map, CoordinateRowList[i], CoordinateColList[i], dir) >= AutoModeLimit)
				{
					if (send_plane_coordinates_with_direction(client, CoordinateRowList[i], CoordinateColList[i], dir) == true)
					{
						score += get_gameprogress_string_double(client, spack);
						if (spack == "HitPlane" || spack == "GameOver")
						{
							ret++;
							set_value_of_plane(map, CoordinateRowList[i], CoordinateColList[i], dir, BDI_VALUE_BODYCLICKED);
							map[CoordinateRowList[i]][CoordinateColList[i]] = BDI_VALUE_HEADCLICKED;
							draw_all_blanks(pCGI, map);
							if (spack == "GameOver")
							{
								return ret;
							}
						}
						if (spack == "HitFail")
						{
							blocked[CoordinateRowList[i]][CoordinateColList[i]][dir] = true;
						}
					}
				}

			}
		}
	}
	return ret;
}

//��map_full���ã����ĳ��ֵ�Ƿ񱻱�ǹ�
static bool check_full_compare(int num)
{
	if (num == BDI_VALUE_BLANK)
		return false;
	else
		return true;
}

//�жϵ�ͼ�Ƿ�����
static bool map_full(const int map[][ColNum])
{
	return map_is_full(map, RowNum, ColNum, check_full_compare);
}

//������״̬������Ϣ
static void update_top_status(const CONSOLE_GRAPHICS_INFO *const pCGI, const int score, const string &spack)
{
	char scoreStr[65] = "", message[256] = "������";
	strcat(message, _itoa(score, scoreStr, 10));
	strcat(message, "  ���ص���Ϣ��");
	strcat(message, spack.c_str());
	strcat(message, "  ");
	gmw_status_line(pCGI, TOP_STATUS_LINE, message);
}

//�ֶ���Ϸģʽ�е����Ϸ���
static int manual_mode_game_left_button(const CONSOLE_GRAPHICS_INFO *const pCGI, int map[][ColNum], int &hitted, const string &spack, const int row, const int col)
{
	if (spack == "HitBody")
	{
		gmw_draw_block(pCGI, row, col, BDI_VALUE_BODY, bdi);
		map[row][col] = BDI_VALUE_BODY;
	}
	if (spack == "HitHead")
	{
		gmw_draw_block(pCGI, row, col, BDI_VALUE_HEAD, bdi);
		if (map[row][col] != BDI_VALUE_HEAD)
			++hitted;
		map[row][col] = BDI_VALUE_HEAD;
	}
	if (spack == "HitFail")
	{
		gmw_draw_block(pCGI, row, col, BDI_VALUE_WRONG, bdi);
		map[row][col] = BDI_VALUE_WRONG;
	}
	return 0;
}

//�ֶ���Ϸģʽ�е������Ҽ�
static int manual_mode_game_right_button(const CONSOLE_GRAPHICS_INFO *const pCGI, int map[][ColNum], int &hitted, const string &spack, const int headRow, const int headCol, const int row, const int col)
{
	if (spack == "HitPlane" || spack == "GameOver")
	{
		gmw_draw_block(pCGI, headRow, headCol, BDI_VALUE_HEADCLICKED, bdi);
		gmw_draw_block(pCGI, row, col, BDI_VALUE_TAIL, bdi);
		if (map[headRow][headCol] != BDI_VALUE_HEAD)
			++hitted;
		map[headRow][headCol] = BDI_VALUE_HEADCLICKED;
		map[row][col] = BDI_VALUE_TAIL;
		if (hitted >= Planes)
			return -1;
	}
	if (spack == "HitFail" || spack == "HitRepeat")
	{
		gmw_draw_block(pCGI, headRow, headCol, map[headRow][headCol], bdi);
	}
	if (spack == "GameOver")
	{
		return -1;
	}
	return 0;
}

//�ֶ���Ϸģʽ
static int manual_mode_game(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client)
{
	int row, headRow, col, headCol, score = 100, ret, maction, keycode1, keycode2, hitted = 0, map[RowNum][ColNum] = { 0 };
	bool doubleClickSelected = false;
	string spack;
	score += get_gameprogress_string_single(client, spack);
	if (spack == "HitBody")
		score -= 1;
	if (spack != "StartGame")
		return -1;
	while (1)
	{
		update_top_status(pCGI, score, spack);
		ret = gmw_read_keyboard_and_mouse(pCGI, maction, row, col, keycode1, keycode2, true);
		if (ret == CCT_MOUSE_EVENT)
		{
			if (maction == MOUSE_LEFT_BUTTON_CLICK)	//������
			{
				if (doubleClickSelected == true)
					gmw_draw_block(pCGI, headRow, headCol, map[headRow][headCol], bdi);
				doubleClickSelected = false;
				client.send_coordinate(char(row + 'A'), col);
				score += get_gameprogress_string_single(client, spack);
				if (spack == "HitBody")
					score -= 1;
				if (manual_mode_game_left_button(pCGI, map, hitted, spack, row, col) < 0)
					break;
				continue;
			}
			if (maction == MOUSE_RIGHT_BUTTON_CLICK)	//����Ҽ�
			{
				if (doubleClickSelected == false)	//��һ��
				{
					headRow = row;
					headCol = col;
					gmw_draw_block(pCGI, row, col, BDI_VALUE_CLICKED, bdi);
				}
				else	//�ڶ���
				{
					client.send_plane_coordinates(char(headRow + 'A'), headCol, char(row + 'A'), col);
					score += get_gameprogress_string_double(client, spack);
					if (manual_mode_game_right_button(pCGI, map, hitted, spack, headRow, headCol, row, col) < 0)
						break;
				}
				doubleClickSelected = !doubleClickSelected;
				continue;
			}
		}
	}
	update_top_status(pCGI, score, spack);
	return 0;
}

//�Զ���Ϸģʽ�н������ɻ���ȫò�ҳ�
static int auto_mode_complete_one_plane(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client, string &spack, int map[][ColNum], int &score, const int row, const int col)
{
	//if (!((row >= 1 && map[row - 1][col] == BDI_VALUE_BODY) || (row < RowNum - 1 && map[row + 1][col] == BDI_VALUE_BODY) || (col >= 1 && map[row][col - 1] == BDI_VALUE_BODY) || (col < ColNum - 1 && map[row][col + 1] == BDI_VALUE_BODY)))	//�����ͷ��Χ�ĸ�û�л����Ǿ�����ȷ�Ϸɻ��ķ���
	//{
	//	int offsetX[] = { 0, 0, -1, 1 }, offsetY[] = { -1, 1, 0, 0 };
	//	for (int k = 0; k < sizeof(offsetY) / sizeof(offsetY[0]); k++)	//���������ҵ�λ�õĿո�
	//	{
	//		if (row_col_in_map(row + offsetY[k], col + offsetX[k]) && map[row + offsetY[k]][col + offsetX[k]] == BDI_VALUE_BLANK)
	//		{
	//			auto_mode_game_one_step(pCGI, client, spack, map, score, row + offsetY[k], col + offsetX[k]);
	//			Sleep(DelayAuto);
	//			if (map[row + offsetY[k]][col + offsetX[k]] == BDI_VALUE_BODY)	//�ҵ��˻���
	//				break;
	//		}
	//	}
	//}

	int bodies[4] = { 0, 0, 0, 0 };	//ͳ��ÿ������Ļ�������
	for (int dir = 0; dir < 4; dir++)
	{
		bodies[dir] = body_value_of_plane(map, row, col, dir);
	}

	int rank[4] = { -1, -1, -1, -1 };	//��ͬ�����ϵĻ����������������
	for (int k = 0; k < 4; k++)
	{
		bool flag = false;
		int maxN = max(max(max(bodies[0], bodies[1]), bodies[2]), bodies[3]);
		if (maxN < 0)
			break;
		for (int i = 0; i < 4; i++)
		{
			if (maxN == bodies[i] && !flag)
			{
				rank[k] = i;
				bodies[i] = -1;
				flag = true;
			}
		}
	}
	
	for (int k = 0; k < 4; k++)	//��������Ľ������˫����
	{
		if (rank[k] < 0)
			break;
		if (send_plane_coordinates_with_direction(client, row, col, rank[k]) == false)
			continue;
		score += get_gameprogress_string_double(client, spack);
		update_top_status(pCGI, score, spack);
		if (spack == "HitPlane" || spack == "GameOver")
		{
			set_value_of_plane(map, row, col, rank[k], BDI_VALUE_BODYCLICKED);
			map[row][col] = BDI_VALUE_HEADCLICKED;
			return 1;
		}
	}
	return 0;
}

//�Զ���Ϸģʽ�д�һ�������ϵĵ㣬����ҵ��˻�ͷ���ͷ���1�����򷵻�0
static int auto_mode_game_one_step(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client, string &spack, int map[][ColNum], int &score, const int row, const int col)
{
	if (!row_col_in_map(row, col))
		return 0;
	if (map[row][col] != BDI_VALUE_BLANK)
		return 0;
	client.send_coordinate(char(row + 'A'), col);
	map[row][col] = get_gameprogress_string_single(client, spack);
	score += map[row][col];
	if (spack == "HitBody")
		score -= 1;
	map[row][col] = map[row][col];
	gmw_draw_block(pCGI, row, col, map[row][col], bdi);
	update_top_status(pCGI, score, spack);
	Sleep(DelayAuto);
	if (map[row][col] == BDI_VALUE_HEAD)
	{
		return 1;
	}
	return 0;
}

//�Զ���Ϸģʽ���õ�����Ѱ������λ��
static void auto_mode_find_heads(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client, string &spack, int map[][ColNum], int &score, int &hitted)
{
	for (int i = 0; i < sizeof(CoordinateRowList) / sizeof(CoordinateRowList[0]); i ++)
	{
		hitted += auto_mode_game_one_step(pCGI, client, spack, map, score, CoordinateRowList[i], CoordinateColList[i]);				//�򿪵�ǰ����
		if (hitted >= 3)
			return;
		hitted += find_planes(pCGI, client, spack, map, score);
		if (spack == "GameOver")
			return;
	}
	
	//����
	////int kX[] = { 3, 6, 2, 7, 0, 9 }, kY[] = { 3, 6, 2, 7, 0, 9 };
	//int kX[] = { 2, 7, 0, 9 }, kY[] = { 2, 7, 0, 9 };
	////int kX[] = { 0, 9 }, kY[] = { 0, 9 };
	//for (int k = 0; k < sizeof(kX) / sizeof(kX[0]) / 2; k++)
	//{
	//	for (int a = 0; a < 4; a++)		//��2*2��С����Ϊ��λѭ��
	//	{
	//		for (int j = kX[2 * k] + a / 2; j <= kX[2 * k + 1]; j += 2)			//ÿ�μ��һ�����ӱ�������
	//		{
	//			for (int i = kY[2 * k] + a % 2; i <= kY[2 * k + 1]; i += 2)		//ÿ�μ��һ�����ӱ�������
	//			{
	//				if (row_col_in_map_head(i, j) == false)
	//					continue;
	//				hitted += auto_mode_game_one_step(pCGI, client, spack, map, hitted, i, j);					//�򿪵�ǰ����
	//				if (hitted >= Planes)
	//					return;
	//				if (row_col_in_map(i - 2, j) && map[i - 2][j] == BDI_VALUE_BODY && map[i][j] == BDI_VALUE_BODY)	//����
	//				{
	//					//if (auto_mode_game_one_step(pCGI, client, spack, map, score, hitted, i - 1, j) < 0)			//�򿪵�ǰ�����ϵ�һ������
	//					//	return;
	//					if (row_col_in_map(i - 4, j) && map[i - 4][j] == BDI_VALUE_BODY && map[i - 3][j] == BDI_VALUE_BODY && map[i - 2][j] == BDI_VALUE_BODY && map[i - 1][j] == BDI_VALUE_BODY && map[i][j] == BDI_VALUE_BODY)	//���ͬһ����������������ǻ���
	//					{
	//						hitted += auto_mode_game_one_step(pCGI, client, spack, map, hitted, i - 2, j - 1);	//��������ߵĸ���
	//						if (hitted >= Planes)
	//							return;
	//						if (j >= 1 && map[i - 2][j - 1] == BDI_VALUE_HEAD)				//�ҵ���һ����ͷ
	//							continue;
	//						hitted += auto_mode_game_one_step(pCGI, client, spack, map, hitted, i - 2, j + 1);	//�������ұߵĸ���
	//							if (hitted >= Planes)
	//								return;
	//						if (j < ColNum - 1 && map[i - 2][j + 1] == BDI_VALUE_HEAD)		//�ҵ���һ����ͷ
	//							continue;
	//					}
	//					//if (map[i - 1][j] == BDI_VALUE_WRONG)		//�����������ǿո�
	//					//{
	//					//	//const int offsetY[] = { -3, -3, -1, -1, 1, 1, 3, 3 }, offsetX[] = { -3, 1, -3, 1, -3, 1, -3, 1 };
	//					//	const int offsetY[] = { -3, -3, 1, 1 }, offsetX[] = { -1, 1, -1, 1 };
	//					//	for (int k = 0; k < sizeof(offsetY) / sizeof(offsetY[0]); k++)	//��ͷ�ܿ��ܳ����ڰ˸�λ���е�һ��
	//					//	{
	//					//		if (row_col_in_map_head(i + offsetY[k], j + offsetX[k]))
	//					//		{
	//					//			if (auto_mode_game_one_step(pCGI, client, spack, map, score, hitted, i + offsetY[k], j + offsetX[k]) < 0)
	//					//				return;
	//					//			if (map[i + offsetY[k]][j + offsetX[k]] == BDI_VALUE_HEAD)		//�ҵ���һ����ͷ
	//					//				continue;
	//					//		}
	//					//	}
	//					//}
	//					//if (map[i - 1][j] == BDI_VALUE_BODY)		//�����������ǻ���
	//					//{
	//					//	const int offsetY[] = { -3, 1 }, offsetX[] = { 0, 0 };
	//					//	//const int offsetY[] = { -3, 1, -1, 1, -1, -3, 3 }, offsetX[] = { 0, 0, 0, -2, -2, -1, -1 };
	//					//	for (int k = 0; k < sizeof(offsetY) / sizeof(offsetY[0]); k++)
	//					//	{
	//					//		if (row_col_in_map_head(i + offsetY[k], j + offsetX[k]))
	//					//		{
	//					//			if (auto_mode_game_one_step(pCGI, client, spack, map, score, hitted, i + offsetY[k], j + offsetX[k]) < 0)
	//					//				return;
	//					//			if (map[i + offsetY[k]][j + offsetX[k]] == BDI_VALUE_HEAD)		//�ҵ���һ����ͷ
	//					//				continue;
	//					//		}
	//					//	}
	//					//}
	//				}
	//				if (row_col_in_map(i, j - 2) && map[i][j - 2] == BDI_VALUE_BODY && map[i][j] == BDI_VALUE_BODY)	//����
	//				{
	//					/*if (auto_mode_game_one_step(pCGI, client, spack, map, score, hitted, i, j - 1) < 0)
	//						return;*/
	//					if (row_col_in_map(i, j - 4))
	//					{
	//						if (map[i][j - 4] == BDI_VALUE_BODY && map[i][j - 3] == BDI_VALUE_BODY && map[i][j - 2] == BDI_VALUE_BODY && map[i][j - 1] == BDI_VALUE_BODY && map[i][j] == BDI_VALUE_BODY)
	//						{
	//							hitted += auto_mode_game_one_step(pCGI, client, spack, map, hitted, i - 1, j - 2);
	//							if (hitted >= Planes)
	//								return;
	//							if (i >= 1 && map[i - 1][j - 2] == BDI_VALUE_HEAD)
	//								continue;
	//							hitted += auto_mode_game_one_step(pCGI, client, spack, map, hitted, i + 1, j - 2);
	//							if (hitted >= Planes)
	//								return;
	//							if (i < ColNum - 1 && map[i + 1][j - 2] == BDI_VALUE_HEAD)
	//								continue;
	//						}
	//					}
	//					//if (map[i][j - 1] == BDI_VALUE_WRONG)	//�����������ǿո�
	//					//{
	//					//	//const int offsetX[] = { -3, -3, -1, -1, 1, 1, 3, 3 }, offsetY[] = { -3, 1, -3, 1, -3, 1, -3, 1 };
	//					//	const int offsetX[] = { -3, -3, 1, 1 }, offsetY[] = { -1, 1, -1, 1 };
	//					//	for (int k = 0; k < sizeof(offsetY) / sizeof(offsetY[0]); k++)
	//					//	{
	//					//		if (row_col_in_map_head(i + offsetY[k], j + offsetX[k]))
	//					//		{
	//					//			if (auto_mode_game_one_step(pCGI, client, spack, map, score, hitted, i + offsetY[k], j + offsetX[k]) < 0)
	//					//				return;
	//					//			if (map[i + offsetY[k]][j + offsetX[k]] == BDI_VALUE_HEAD)
	//					//				continue;
	//					//		}
	//					//	}
	//					//}
	//					//if (map[i][j - 1] == BDI_VALUE_BODY)	//�����������ǻ���
	//					//{
	//					//	const int offsetX[] = { -3, 1 }, offsetY[] = { 0, 0 };
	//					//	//const int offsetX[] = { -3, 1, -1, 1, -1, -3, 3 }, offsetY[] = { 0, 0, 0, -2, -2, -1, -1 };
	//					//	for (int k = 0; k < sizeof(offsetY) / sizeof(offsetY[0]); k++)
	//					//	{
	//					//		if (row_col_in_map_head(i + offsetY[k], j + offsetX[k]))
	//					//		{
	//					//			if (auto_mode_game_one_step(pCGI, client, spack, map, score, hitted, i + offsetY[k], j + offsetX[k]) < 0)
	//					//				return;
	//					//			if (map[i + offsetY[k]][j + offsetX[k]] == BDI_VALUE_HEAD)
	//					//				continue;
	//					//		}
	//					//	}
	//					//}
	//				}
	//			}
	//		}
	//	}
	//}

	//������
	/*while (1)
	{
		int _row, _col;
		do
		{
			_row = rand() % RowNum;
			_col = rand() % ColNum;
		} while (row_col_in_map_head(_row, _col) && map[_row][_col] != BDI_VALUE_BLANK && !map_full(map));
		if (auto_mode_game_one_step(pCGI, client, spack, map, score, hitted, _row, _col) < 0)
			return;
	}*/
}

//�Զ���Ϸģʽ
static int auto_mode_game(const CONSOLE_GRAPHICS_INFO *const pCGI, cmd_tcp_socket &client)
{
	int score = 100, hitted = 0, map[RowNum][ColNum] = { 0 };
	string spack;
	get_gameprogress_string_single(client, spack);
	if (spack != "StartGame")
		return -1;

	auto_mode_find_heads(pCGI, client, spack, map, score, hitted);	//Ѱ�����л�ͷ

	for (int i = 0; i < sizeof(CoordinateColList) / sizeof(CoordinateColList[0]); i++)	//���ݻ�ͷλ�û������зɻ�
	{
		if (map[CoordinateRowList[i]][CoordinateColList[i]] == BDI_VALUE_HEAD)
		{
			auto_mode_complete_one_plane(pCGI, client, spack, map, score, CoordinateRowList[i], CoordinateColList[i]);
			update_top_status(pCGI, score, spack);
		}
	}
	update_top_status(pCGI, client.get_score(), spack);
	draw_all_blanks(pCGI, map);
	ofstream out;
	out.open("out.txt", ios::out | ios::app);
	out << client.get_score() << endl;
	out.close();
	return 0;
}

//��Ϸ��ʼǰ��׼������
static void game_prepare(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	cls();
	gmw_init(pCGI, RowNum, ColNum, COLOR_BLACK, COLOR_HWHITE);
	pCGI->other_setting_enable_right_mouse = 1;
	gmw_set_frame_style(pCGI, 2, 1, true);
	gmw_set_font(pCGI, FontName, FontSize);
	gmw_set_frame_color(pCGI, COLOR_HWHITE, COLOR_BLACK);
	gmw_set_frame_default_linetype(pCGI, 1);
	gmw_set_ext_rowcol(pCGI, 0, ExternRow, 0, ExternCol);
	gmw_set_colno_switch(pCGI, true);
	gmw_set_rowno_switch(pCGI, true);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, true);
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, true);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, COLOR_BLACK, COLOR_HWHITE, COLOR_BLACK, COLOR_HYELLOW);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE, COLOR_BLACK, COLOR_HWHITE, COLOR_BLACK, COLOR_HYELLOW);
	gmw_draw_frame(pCGI);
}

//��Ϸ��ʼ
int game_start(cmd_tcp_socket &client, const bool autoMode)
{
	CONSOLE_GRAPHICS_INFO CGI;
	setcursor(CURSOR_INVISIBLE);
	game_prepare(&CGI);
	setcolor();
	showstr(CGI.extern_left_cols, CGI.extern_up_lines + (CGI.CFI.block_high + 1) * CGI.row_num + 4, HelpInfo, COLOR_BLACK, COLOR_HWHITE);
	int ret;
	if (autoMode)
	{
		ret = auto_mode_game(&CGI, client);
	}
	else
	{
		enable_mouse();
		ret = manual_mode_game(&CGI, client);
		disable_mouse();
	}
	setcursor(CURSOR_SHOWING);
	return ret;
}

//�����Ϸ������Ľ��沢�ȴ��û����룬����1��ʾ�ؿ�������0��ʾ�˳�
int game_over(bool autoMode)
{
	const int offsetX = RowNum / 2 * 4 - 4, offsetY = 10;
	const char choiceList[] = "RrQq";
	showstr(offsetX, offsetY - 2, "�X�T�T�T�T�T�[", COLOR_PINK, COLOR_HWHITE);
	showstr(offsetX, offsetY - 1, "�U ��Ϸ���� �U", COLOR_PINK, COLOR_HWHITE);
	showstr(offsetX, offsetY + 0, "�U R���ؿ�  �U", COLOR_PINK, COLOR_HWHITE);
	showstr(offsetX, offsetY + 1, "�U Q���˳�  �U", COLOR_PINK, COLOR_HWHITE);
	showstr(offsetX, offsetY + 2, "�^�T�T�T�T�T�a", COLOR_PINK, COLOR_HWHITE);
	setcolor();
	if (!autoMode || (autoMode && AutoModeRetrySwitch))
	{
		char ch = wait_enter(choiceList);
		if (toupper(ch) == 'R')
			return 1;
		if (toupper(ch) == 'Q')
			return 0;
	}
	return 0;
}
