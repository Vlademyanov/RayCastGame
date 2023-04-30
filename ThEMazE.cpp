#include <iostream>
#include <windows.h>
#include <chrono>
#include <vector>
#include <algorithm>

int main(){

	POINT p;						// Точка р для определения коорд мыши
	const int X = 960;				// Размер экрана по X 960
	const int Y = 227;				// Размер экрана по Y 206
	const int WSX = 64;				// Размер мира по X
	const int WSY = 64;				// Размер мира по Y
	const int Space = 30;			// Высота "залазания"
	const float PI = 3.14159f;		// pi
	float PosX = 29.0f;				// Координаты игрока по X
	float PosY = 2.0f;				// Координаты игрока по Y
	float PosA = 0.0f;				// Начальный поворот взгляда игрока
	const float FOV = PI / 2.0f;	// Поле зрения
	const float DEPTH = 16.0f;		// Длина прорисовки
	const float Step = 0.01f;		// Шаг луча
	float Speed = 5.5f;				// Скорость игрока


	// Создание буфера
	wchar_t* Plane = new wchar_t[X * Y];
	HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	DWORD Painted = 0;


	// Мир
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

	// Временные точки
	auto Time = std::chrono::system_clock::now();
	auto OldTime = std::chrono::system_clock::now();

	/*ИГРОВОЙ ЦИКЛ*/

	while (true){

		// Разница во времени на кадр для вычисления модификации скорости движения
		OldTime = std::chrono::system_clock::now();
		std::chrono::duration<float> TimeDif = OldTime - Time;
		Time = OldTime;
		float FPS = TimeDif.count();
		
		/*УПРАВЛЕНИЕ*/

		// Поворот мышью
		if (GetCursorPos(&p))
			if (p.y > 150 && p.y < 900){
				if (p.x < 150)						PosA -= (Speed * 0.35f) * FPS;		// Лево резко
				else if (p.x < 450)					PosA -= (Speed * 0.15f) * FPS;		// Лево
				else if (p.x > 1470 && p.x < 1820)	PosA += (Speed * 0.15f) * FPS;		// Право
				else if (p.x > 1770)				PosA += (Speed * 0.35f) * FPS;		// Право резко
			}

		// Поворот стрелочками
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)		// Лево
			PosA -= 0.45f * Speed * FPS;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	// Право
			PosA += 0.45f * Speed * FPS;

		// Управление WASD и коллизии
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

		// Скорость движения в зависимости от нажатия Shift и использования карты
		if (GetAsyncKeyState('M') & 0x8000)				Speed = 1.5f;
		else if (GetAsyncKeyState(VK_SHIFT) & 0x8000)	Speed = 7.5f;
		else											Speed = 5.5f;

		// Рестарт
		if (GetAsyncKeyState('R') & 0x8000) {
			PosX = 29.0f;
			PosY = 2.0f;
			PosA = 0.0f;
		}
		// Выход
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return 0;

		/*МАТЕМАТИКА ДЛЯ ОТОБРАЖЕНИЯ*/

		for (int x = 0; x < X; x++) {
			float AngleBite = (PosA - FOV / 2.0f) + (x / (float)X) * FOV;

			/*Дистанция до стены*/
				  										
			float DistW = 0.0f;
			float DistS = 0.0f;

			//bool SlabY = false;
			bool Wall = false;				// Попадание в стену
			bool Slab = false;				// Попадание в полублок
			bool Fringe = false;			// Попадание в грань
			bool FringeS = false;			// Попадание в грань полублока

			float UnitX = sinf(AngleBite);	// Сост. единичного ветора по X для движения вдоль него при шагах
			float UnitY = cosf(AngleBite);	// Сост. единичного ветора по Y для движения вдоль него при шагах

			// Идём до стены
			while (!Wall && DistW < DEPTH) {
				DistW += Step; 
				int WorldXI = (int)(UnitX * DistW / cosf(PosA - AngleBite) + PosX);		// Координаты кончика луча по X
				int WorldYI = (int)(UnitY * DistW / cosf(PosA - AngleBite) + PosY);		// Координаты кончика луча по Y

				// Проверка на out of bounds
				if (WorldXI < 0 || WorldXI >= WSX || WorldYI < 0 || WorldYI >= WSY) {
					Wall = true;			
					DistW = DEPTH;
				}
				else{

					// Проверка на пересечение с блоком
					if (WORLD.c_str()[WorldYI + WorldXI * WSX] == '#') {
						Wall = true;

						/*ШТРИХОВКА РЁБЕР СТЕН*/

						std::vector<std::pair<float, float>> p;

						// Проверяем кадый угол блока запоминая расстояние 
						// и точечное произведение двух лучей
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++) {
								float vy = WorldYI + ty - PosY;
								float vx = WorldXI + tx - PosX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (UnitX * vx / d) + (UnitY * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

						// Сортируем рёбра от ближайшего до самого дальнего 
						sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// Ближайшие ребра
						float BoundAngle = 0.005;
						if (acos(p.at(0).second) < BoundAngle) Fringe = true;
						if (acos(p.at(1).second) < BoundAngle) Fringe = true;
					}
				}
			}
			while (!Slab && DistS < DEPTH) {
				DistS += Step;
				int WorldXI = (UnitX * DistS / cosf(PosA - AngleBite) + PosX);		// Координаты кончика луча по X
				int WorldYI = (UnitY * DistS / cosf(PosA - AngleBite) + PosY);		// Координаты кончика луча по Y

				// Проверка на out of bounds
				if (WorldXI < 0 || WorldXI >= WSX || WorldYI < 0 || WorldYI >= WSY) {
					Slab = true;			
					DistS = DEPTH;
				}
				else {

					// Проверка на пересечение с полублоком
					if (WORLD.c_str()[WorldYI + WorldXI * WSX] == '/' || WORLD.c_str()[WorldYI + WorldXI * WSX] == '#') {
						Slab = true;
						//if (WORLD.c_str()[WorldYI + WorldXI * WSX] == '/')
						//	SlabY = true;
					
						/*ШТРИХОВКА РЁБЕР ПОЛУБЛОКОВ*/

						std::vector<std::pair<float, float>> p;

						// Проверяем кадый угол блока запоминая расстояние 
						// и точечное произведение двух лучей
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++){
								float vy = WorldYI + ty - PosY;
								float vx = WorldXI + tx - PosX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (UnitX * vx / d) + (UnitY * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

						// Сортируем рёбра от ближайшего до самого дальнего 
						sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// Ближайшие ребра
						float BoundAngle = 0.005;
						if (acos(p.at(0).second) < BoundAngle) FringeS = true;
						if (acos(p.at(1).second) < BoundAngle) FringeS = true;
					}
				}
			}

			/*ОТОБРАЖЕНИЕ*/

			// Длина до потолка и пола 
			int Sky = (Y / 2.0) - Y / (DistS);
			int Ground = Y - Sky;
			Sky = (Y / 2.0) - Y / (DistW);
						
			// Подъём камеры при перелазании
			int SkyS = Sky + Space;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '/') {
				while (Sky < SkyS) {
					Sky += 1 + Speed * (int)FPS;
				}
			}

			/*ШТРИХОВКА*/
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
				
				if (y <= Sky) Plane[y * X + x] = ' ';								// Небо
				else if (y > Sky && y <= Y / 2.0) Plane[y * X + x] = Shade;			// Стена
				else if (y < (Y + 7) / 2.0 && y > (Y - 7) / 2.0						// Ребро для полублока
					&& WORLD.c_str()[(int)PosY + (int)PosX * WSX] != '/')	
					Plane[y * X + x] = ShadeF;
				else if (y > Y / 2.0 && y <= Ground) {								// Полублок
					if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '/')
						Ground = Y - Sky + 2 * Space;
						Plane[y * X + x] = Shade;
					if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] != '/')
						Plane[y * X + x] = ShadeS;
				}
				else {																// Земля
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

		// Отображение карты
		if (GetAsyncKeyState('M') & 0x8000)
			for (int x = 0; x < WSX; x++)
				for (int y = 0; y < WSX; y++){
					Plane[(y)*X + x + 83] = WORLD[y * WSX + x];
					Plane[((int)PosX) * X + (int)PosY + 83] = 'V';
				}

		// Вывод кадра
		Plane[X * Y - 1] = '\0';
		WriteConsoleOutputCharacter(Console, Plane, X * Y, { 0,0 }, &Painted);
	}
	return 0;
}

//if (WorldXI >= 2 && WorldXI <= 63 && WorldYI >= 2 && WorldYI <= 63) {					// Ноль рёбер 
						//	if (WORLD.c_str()[(WorldXI + 1) * WSX + (WorldYI)] == '#' &&
						//		WORLD.c_str()[(WorldXI - 1) * WSX + (WorldYI)] == '#' ||
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI + 1)] == '#' &&
						//		WORLD.c_str()[(WorldXI) * WSX + (WorldYI - 1)] == '#') {
						//	}
						//	if (WORLD.c_str()[(WorldXI + 1) * WSX + (WorldYI)] == '#' &&					// Одно ребро
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
						//	//if (WORLD.c_str()[(WorldXI + 1) * WSX + (WorldYI)] == '#' ||					// Два ребра
						//	//	WORLD.c_str()[(WorldXI - 1) * WSX + (WorldYI)] == '#' ||
						//	//	WORLD.c_str()[(WorldXI) * WSX + (WorldYI + 1)] == '#' ||
						//	//	WORLD.c_str()[(WorldXI) * WSX + (WorldYI - 1)] == '#' ) {
						//	//	if (acos(p.at(0).second) < Bound) Fringe = true;
						//	//	if (acos(p.at(1).second) < Bound) Fringe = true;
						//	//}
						//}
						//else {	}				//Три ребра
