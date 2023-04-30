#include <iostream>
#include <windows.h>
#include <chrono>
#include <vector>
#include <algorithm>

int main(){

	POINT p;						// ����� � ��� ����������� ����� ����
	const int X = 960;				// ������ ������ �� X 960
	const int Y = 227;				// ������ ������ �� Y 206
	const int WSX = 64;				// ������ ���� �� X
	const int WSY = 64;				// ������ ���� �� Y
	const int Space = 30;			// ������ "���������"
	const float PI = 3.14159f;		// pi
	float PosX = 29.0f;				// ���������� ������ �� X
	float PosY = 2.0f;				// ���������� ������ �� Y
	float PosA = 0.0f;				// ��������� ������� ������� ������
	const float FOV = PI / 2.0f;	// ���� ������
	const float DEPTH = 16.0f;		// ����� ����������
	const float Step = 0.01f;		// ��� ����
	float Speed = 5.5f;				// �������� ������


	// �������� ������
	wchar_t* Plane = new wchar_t[X * Y];
	HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	DWORD Painted = 0;


	// ���
	std::wstring WORLD;
	WORLD += L"################################################################";
	WORLD += L"################################################################";
	WORLD += L"################################################################";
	WORLD += L"################################################################";
	WORLD += L"#####...........................................##..........####";
	WORLD += L"#####...........................................##..........####";
	WORLD += L"#####...######..##..##################..######..##########..####";
	WORLD += L"#####...######..##..##################..######..##########..####";
	WORLD += L"#####...........##......##..............##..................####";
	WORLD += L"#####...........##......##..............##..................####";
	WORLD += L"#####...######################..##########..##..################";
	WORLD += L"#####...######################..##########..##..################";
	WORLD += L"#####...................##..........##......##..##..........####";
	WORLD += L"#####...................##..........##......##..##..........####";
	WORLD += L"######################..##############..######..######..##..####";
	WORLD += L"######################..##############..######..######..##..####";
	WORLD += L"#####...........##..//..##..........##..##..##..........##..####";
	WORLD += L"#####...........##..##..##..........##..##..##..........##..####";
	WORLD += L"#####...######..##..##..##..######..##..##..##########..##..####";
	WORLD += L"#####...######..##..##..##..######..##..##..##########..##..####";
	WORLD += L"#####...##..##..##..##......##..##..##......##......##..##..####";
	WORLD += L"#####...##..##..##..##......##..##..##......##......##..##..####";
	WORLD += L"#####...##..##..##..##########..##..##########..##..##..##/#####";
	WORLD += L"#####...##..##..##..##########..##..##########..##..##..##/#####";
	WORLD += L"#####...##......##..............##..............##..##..##..####";
	WORLD += L"#####...##......##..............##..............##..##..##..####";
	WORLD += L"#####...##..######..##########..##############..##..##..##..####";
	WORLD += L"#####...##..######..##########..##############..##..##..##..####";
	WORLD += L"##......//..##..............##..............##..##..##..##......";
	WORLD += L"##......##..##......##......##..............##..##..##..##......";
	WORLD += L"##########..##########..##..............##..##..##..##..##..####";
	WORLD += L"##########..##########..##....##...//...##..##..##..##..##..####";
	WORLD += L"#####...##..........##..##....##...//...##......##..##..##..####";
	WORLD += L"#####...##..........##..##..............##......##..//..##..####";
	WORLD += L"#####...##########..##..##############################..##..####";
	WORLD += L"#####...##########..##..##############################..##..####";
	WORLD += L"#####...............##......................##..........##..####";
	WORLD += L"#####...............##......................##..........##..####";
	WORLD += L"##############..##..##/###############..##..######..######..####";
	WORLD += L"##############..##..##/###############..##..######..######..####";
	WORLD += L"#####...........##..##..........##......##......##......##..####";
	WORLD += L"#####...........##..##..........##......##......##......##..####";
	WORLD += L"#####...##//######..########....##############..##..##..##..####";
	WORLD += L"#####...##//######..########....##############..##..##..##..####";
	WORLD += L"#####...##..........##..##..............##..##..##..##..##..####";
	WORLD += L"#####...##..........##..##..............##..##..##..##..##..####";
	WORLD += L"#####...##..##..######..##..##########..##..##..##..######..####";
	WORLD += L"#####...##..##..######..##..##########..##..##..##..######..####";
	WORLD += L"#####...##..##..........##..##..............##..##..........####";
	WORLD += L"#####...##..##..........##..##..............##..##..........####";
	WORLD += L"#####...##..##############..##..##..##########..##########/#####";
	WORLD += L"#####...##..##############..##..##..##########..##########/#####";
	WORLD += L"#####...##..............##..##..##......##..............##..####";
	WORLD += L"#####...##..............##..##..##......##..............##..####";
	WORLD += L"#####...##############..##..##..##..######..##########..##..####";
	WORLD += L"#####...##############..##..##..##..######..##########..##..####";
	WORLD += L"#####...............##......##..............##..............####";
	WORLD += L"#####...............##......##..............##..............####";
	WORLD += L"################################################################";
	WORLD += L"###############....##....#####....#####........#################";
	WORLD += L"###############..##..##..####..##..####..######..###############";
	WORLD += L"###############..######..###........###........#################";
	WORLD += L"###############..######..##..######..##..#######################";
	WORLD += L"################################################################";

	// ��������� �����
	auto Time = std::chrono::system_clock::now();
	auto OldTime = std::chrono::system_clock::now();

	/*������� ����*/

	while (true){

		// ������� �� ������� �� ���� ��� ���������� ����������� �������� ��������
		OldTime = std::chrono::system_clock::now();
		std::chrono::duration<float> TimeDif = OldTime - Time;
		Time = OldTime;
		float FPS = TimeDif.count();
		
		/*����������*/

		// ������� �����
		if (GetCursorPos(&p))
			if (p.y > 150 && p.y < 900){
				if (p.x < 150)						PosA -= (Speed * 0.35f) * FPS;		// ���� �����
				else if (p.x < 450)					PosA -= (Speed * 0.15f) * FPS;		// ����
				else if (p.x > 1470 && p.x < 1820)	PosA += (Speed * 0.15f) * FPS;		// �����
				else if (p.x > 1770)				PosA += (Speed * 0.35f) * FPS;		// ����� �����
			}

		// ������� �����������
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)		// ����
			PosA -= 0.45f * Speed * FPS;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	// �����
			PosA += 0.45f * Speed * FPS;

		// ���������� WASD � ��������
		if (GetAsyncKeyState('W') & 0x8000){
			PosX += sinf(PosA) * FPS * Speed;
			PosY += cosf(PosA) * FPS * Speed;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '#' /*|| WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '/'*/){
				PosX -= sinf(PosA) * FPS* Speed;
				PosY -= cosf(PosA) * FPS * Speed;
			}
		}
		if (GetAsyncKeyState('A') & 0x8000){
			PosX -= cosf(PosA) * FPS * Speed;
			PosY += sinf(PosA) * FPS * Speed;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '#' /*|| WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '/'*/){
				PosX += cosf(PosA) * FPS * Speed;
				PosY -= sinf(PosA) * FPS * Speed;
			}
		}
		if (GetAsyncKeyState('S') & 0x8000) {
			PosX -= sinf(PosA) * FPS* Speed;
			PosY -= cosf(PosA) * FPS* Speed;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '#' /*|| WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '/'*/) {
				PosX += sinf(PosA) * FPS* Speed;
				PosY += cosf(PosA) * FPS* Speed;
			}
		}
		if (GetAsyncKeyState('D') & 0x8000){
			PosX += cosf(PosA) * FPS* Speed;
			PosY -= sinf(PosA) * FPS* Speed;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '#' /*|| WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '/'*/){
				PosX -= cosf(PosA) * FPS* Speed;
				PosY += sinf(PosA) * FPS* Speed;
			}
		}

		// �������� �������� � ����������� �� ������� Shift � ������������� �����
		if (GetAsyncKeyState('M') & 0x8000)				Speed = 1.5f;
		else if (GetAsyncKeyState(VK_SHIFT) & 0x8000)	Speed = 7.5f;
		else											Speed = 5.5f;

		// �������
		if (GetAsyncKeyState('R') & 0x8000) {
			PosX = 29.0f;
			PosY = 2.0f;
			PosA = 0.0f;
		}
		// �����
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return 0;

		/*���������� ��� �����������*/

		for (int x = 0; x < X; x++) {
			float AngleBite = (PosA - FOV / 2.0f) + (x / (float)X) * FOV;

			/*��������� �� �����*/
				  										
			float DistW = 0.0f;
			float DistS = 0.0f;

			//bool SlabY = false;
			bool Wall = false;				// ��������� � �����
			bool Slab = false;				// ��������� � ��������
			bool Fringe = false;			// ��������� � �����
			bool FringeS = false;			// ��������� � ����� ���������

			float UnitX = sinf(AngleBite);	// ����. ���������� ������ �� X ��� �������� ����� ���� ��� �����
			float UnitY = cosf(AngleBite);	// ����. ���������� ������ �� Y ��� �������� ����� ���� ��� �����

			// ��� �� �����
			while (!Wall && DistW < DEPTH) {
				DistW += Step; 
				int WorldXI = (int)(UnitX * DistW / cosf(PosA - AngleBite) + PosX);		// ���������� ������� ���� �� X
				int WorldYI = (int)(UnitY * DistW / cosf(PosA - AngleBite) + PosY);		// ���������� ������� ���� �� Y

				// �������� �� out of bounds
				if (WorldXI < 0 || WorldXI >= WSX || WorldYI < 0 || WorldYI >= WSY) {
					Wall = true;			
					DistW = DEPTH;
				}
				else{

					// �������� �� ����������� � ������
					if (WORLD.c_str()[WorldYI + WorldXI * WSX] == '#') {
						Wall = true;

						/*��������� Ш��� ����*/

						std::vector<std::pair<float, float>> p;

						// ��������� ����� ���� ����� ��������� ���������� 
						// � �������� ������������ ���� �����
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++) {
								float vy = WorldYI + ty - PosY;
								float vx = WorldXI + tx - PosX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (UnitX * vx / d) + (UnitY * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

						// ��������� ���� �� ���������� �� ������ �������� 
						sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// ��������� �����
						float BoundAngle = 0.005;
						if (acos(p.at(0).second) < BoundAngle) Fringe = true;
						if (acos(p.at(1).second) < BoundAngle) Fringe = true;
					}
				}
			}
			while (!Slab && DistS < DEPTH) {
				DistS += Step;
				int WorldXI = (UnitX * DistS / cosf(PosA - AngleBite) + PosX);		// ���������� ������� ���� �� X
				int WorldYI = (UnitY * DistS / cosf(PosA - AngleBite) + PosY);		// ���������� ������� ���� �� Y

				// �������� �� out of bounds
				if (WorldXI < 0 || WorldXI >= WSX || WorldYI < 0 || WorldYI >= WSY) {
					Slab = true;			
					DistS = DEPTH;
				}
				else {

					// �������� �� ����������� � ����������
					if (WORLD.c_str()[WorldYI + WorldXI * WSX] == '/' || WORLD.c_str()[WorldYI + WorldXI * WSX] == '#') {
						Slab = true;
						//if (WORLD.c_str()[WorldYI + WorldXI * WSX] == '/')
						//	SlabY = true;
					
						/*��������� Ш��� ����������*/

						std::vector<std::pair<float, float>> p;

						// ��������� ����� ���� ����� ��������� ���������� 
						// � �������� ������������ ���� �����
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++){
								float vy = WorldYI + ty - PosY;
								float vx = WorldXI + tx - PosX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (UnitX * vx / d) + (UnitY * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

						// ��������� ���� �� ���������� �� ������ �������� 
						sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// ��������� �����
						float BoundAngle = 0.005;
						if (acos(p.at(0).second) < BoundAngle) FringeS = true;
						if (acos(p.at(1).second) < BoundAngle) FringeS = true;
					}
				}
			}

			/*�����������*/

			// ����� �� ������� � ���� 
			int Sky = (Y / 2.0) - Y / (DistS);
			int Ground = Y - Sky;
			Sky = (Y / 2.0) - Y / (DistW);
						
			// ������ ������ ��� �����������
			int SkyS = Sky + Space;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '/') {
				while (Sky < SkyS) {
					Sky += 1 + Speed * (int)FPS;
				}
			}

			/*���������*/
			short Shade = ' ', ShadeS = ' ', ShadeF = ' ', ShadeK = ' ';
			if (DistW <= DEPTH / 4.0f)			ShadeF = 0x2591;
			else if (DistW < DEPTH / 3.0f)		ShadeF = 0x2591;
			else if (DistW < DEPTH / 2.0f)		ShadeF = 0x2591;
			else if (DistW < DEPTH)				ShadeF = 0x2591;
			else								ShadeF = ' ';

			if (DistW <= DEPTH / 4.0f)			Shade = 0x2588;		
			else if (DistW < DEPTH / 3.0f)		Shade = 0x2593;
			else if (DistW < DEPTH / 2.0f)		Shade = 0x2592;
			else if (DistW < DEPTH)				Shade = 0x2591;
			else								Shade = ' ';	

			if (DistW <= DEPTH / 4.0f)			ShadeK = 0x2588;
			else if (DistW < DEPTH / 3.0f)		ShadeK = 0x2593;
			else if (DistW < DEPTH / 2.0f)		ShadeK = 0x2592;
			else if (DistW < DEPTH)				ShadeK = 0x2591;
			else								ShadeK = ' ';
			if (Fringe)							ShadeK = 0x2591;

			if (DistS <= DEPTH / 4.0f)			ShadeS = 0x2588;
			else if (DistS < DEPTH / 3.0f)		ShadeS = 0x2593;
			else if (DistS < DEPTH / 2.0f)		ShadeS = 0x2592;
			else if (DistS < DEPTH)				ShadeS = 0x2591;
			else								ShadeS = ' ';
			if (FringeS)						ShadeS = 0x2591;
			for (int y = 0; y < Y; y++) {
				
				if (y <= Sky) Plane[y * X + x] = ' ';								// ����
				else if (y > Sky && y <= Y / 2.0) Plane[y * X + x] = Shade;			// �����
				else if (y < (Y + 7) / 2.0 && y > (Y - 7) / 2.0						// ����� ��� ���������
					&& WORLD.c_str()[(int)PosY + (int)PosX * WSX] != '/')	
					Plane[y * X + x] = ShadeF;
				else if (y > Y / 2.0 && y <= Ground) {								// ��������
					if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '/')
						Ground = Y - Sky + 2 * Space;
						Plane[y * X + x] = Shade;
					if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] != '/')
						Plane[y * X + x] = ShadeS;
				}
				else {																// �����
					float DistG = 1.0f - ((y - Y / 2.0f) / (Y / 2.0f));
					if (DistG < 0.25)		Shade = '#';
					else if (DistG < 0.5)	Shade = 'x';
					else if (DistG < 0.75)	Shade = '.';
					else if (DistG < 0.9)	Shade = '-';
					else					Shade = ' ';
					Plane[y * X + x] = Shade;
				}
				if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '/') {
					if (y > Sky && y <= (Y + 3) / 2.0 + Space) Plane[y * X + x] =					Shade;
					
					if (y > (Y - 3) / 2.0 + Space && y <= Ground) Plane[y * X + x] =				ShadeK;
					if (y < (Y + 3) / 2.0 + Space && y >(Y - 3) / 2.0 + Space) Plane[y * X + x] = ShadeF;
				}
			}
		}

		// ����������� �����
		if (GetAsyncKeyState('M') & 0x8000)
			for (int x = 0; x < WSX; x++)
				for (int y = 0; y < WSX; y++){
					Plane[(y)*X + x + 83] = WORLD[y * WSX + x];
					Plane[((int)PosX) * X + (int)PosY + 83] = 'V';
				}

		// ����� �����
		Plane[X * Y - 1] = '\0';
		WriteConsoleOutputCharacter(Console, Plane, X * Y, { 0,0 }, &Painted);
	}
	return 0;
}

//if (WorldXI >= 2 && WorldXI <= 63 && WorldYI >= 2 && WorldYI <= 63) {					// ���� ���� 
						//	if (WORLD.c_str()[(WorldXI + 1) * WSX + (WorldYI)] == '#' &&
						//		WORLD.c_str()[(WorldXI - 1) * WSX + (WorldYI)] == '#' ||
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI + 1)] == '#' &&
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI - 1)] == '#') {
						//	}
						//	if (WORLD.c_str()[(WorldXI + 1) * WSX + (WorldYI)] == '#' &&					// ���� �����
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI + 1)] == '#' &&
						//		WORLD.c_str()[(WorldXI - 1) * WSX + (WorldYI)] == '.' &&
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI - 1)] == '.'
						//		||
						//		WORLD.c_str()[(WorldXI - 1) * WSX + (WorldYI)] == '#' &&
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI - 1)] == '#' &&
						//		WORLD.c_str()[(WorldXI + 1) * WSX + (WorldYI)] == '.' &&
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI + 1)] == '.'
						//		||
						//		WORLD.c_str()[(WorldXI + 1) * WSX + (WorldYI)] == '#' &&
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI - 1)] == '#' &&
						//		WORLD.c_str()[(WorldXI - 1) * WSX + (WorldYI)] == '.' &&
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI + 1)] == '.'
						//		||
						//		WORLD.c_str()[(WorldXI - 1) * WSX + (WorldYI)] == '#' &&
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI + 1)] == '#' &&
						//		WORLD.c_str()[(WorldXI - 1) * WSX + (WorldYI)] == '.' &&
						//		WORLD.c_str()[(WorldXI)*WSX + (WorldYI + 1)] == '.'){
						//		if (acos(p.at(0).second) < Bound) Fringe = true;
						//	}
						//	
						//	//if (WORLD.c_str()[(WorldXI + 1) * WSX + (WorldYI)] == '#' ||					// ��� �����
						//	//	WORLD.c_str()[(WorldXI - 1) * WSX + (WorldYI)] == '#' ||
						//	//	WORLD.c_str()[(WorldXI) * WSX + (WorldYI + 1)] == '#' ||
						//	//	WORLD.c_str()[(WorldXI) * WSX + (WorldYI - 1)] == '#' ) {
						//	//	if (acos(p.at(0).second) < Bound) Fringe = true;
						//	//	if (acos(p.at(1).second) < Bound) Fringe = true;
						//	//}
						//}
						//else {	}				//��� �����
