#include <windows.h>				// Отображение, кнопки
#include <chrono>				// Время
#include <vector>				// Рёбра
#include <algorithm>
#include <sstream>				// Чтение из файла
#include <fstream>
#include <codecvt>
#include <random>				// Рандом
#pragma comment(lib, "winmm.lib")		// Музыка

const int X = 960;				// Размер экрана по X 960
const int Y = 227;				// Размер экрана по Y 206/227

// Структруа сущности
struct Entity {
	std::wstring HealthPanel;		// Панель здоровья
	int X;					// Размер панели здоровья игрока по X 
	int Y;					// Размер панели здоровья игрока по Y 
	int IFX = X / 320;			// Прибавка для размера пользовательского интерфейса по X
	int IFY = Y / 113.5;			// Прибавка для размера пользовательского интерфейса по Y
	float Damage;				// Полученный урон
	float Attack;				// Накосимый урон
	float Health;				// Здоровье
	float MaxHealth;			// Максимальное здоровье
};

// Отрисовка панелей здоровья
void drawHP(wchar_t* Plane, Entity Entity, int Mod) {
	for (int x = 0; x < Entity.X; x++)
		for (int y = 0; y < Entity.Y; y++) {
			Plane[(y + Entity.IFY) * X + x + Entity.IFX] = Entity.HealthPanel[y * Entity.X + x];
			Entity.Damage = (Entity.MaxHealth - int(Entity.Health)) * (int)(Entity.X / Entity.MaxHealth);
			if (Entity.Damage > 0)
				for (int i = 0; i <= Entity.Damage; i++)
					for (int j = Entity.IFY + (int)(Entity.Y / 3); j <= Entity.IFY + (int)(2 * Entity.Y / 3) - 1; j++)
						Plane[j * X + Entity.IFX + Entity.X - (int)(Entity.X / 52) - Mod - i] = ' ';
		}
}

// Отрисовка полноэкранных изображений
void draw(wchar_t* Plane, std::wstring Pict, int PX, int PY) {
	for (int y = 0; y < PY; y++) 
		for (int x = 0; x < PX; x++) 
			Plane[y * X + x] = Pict[y * PX + x];
}

// Отрисовка изображений в центре
void drawCenter(wchar_t* Plane, std::wstring Pict, int PX, int PY) {
	for (int y = 0; y < PY; y++) {
		for (int x = 0; x < PX; x++) {
			//Plane[y * X + x] = Pict[y * PX + x];
			Plane[(y + (Y - PY) / 2) * X + x + (X - PX) / 2] = Pict[y * PX + x];
		}
	}
}

// Рандом
int randomNumber(int min, int max) {
	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(min, max);
	return distribution(generator);
}

// Чтение из файла
std::wstring readFile(const char* filename) {
	std::wifstream wif(filename);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}

int main() {
	POINT p;						// Точка р для определения коорд мыши
	Entity Player;						// Игрок
	Entity Bat;						// враг: Bat
	Player.X = 208;						// Размер панели здоровья игрока по X
	Player.Y = 12;						// Размер панели здоровья игрока по Y
	Bat.X = 68;						// Размер панели здоровья врага по X
	Bat.Y = 12;						// Размер панели здоровья врага по Y
	const int WSX = 64;					// Размер мира по X
	const int WSY = 64;					// Размер мира по Y
	const int MSX = 256;					// Размер карты по X
	const int MSY = 128;					// Размер карты по Y
	const int SpaceF = Y / 32;				// Модификатор для ребра при "залазании"
	const int Space = X / 32;				// Высота "залазания"
	const float PI = 3.14159f;				// pi
	const float FOV = PI / 2;				// Поле зрения
	const float DEPTH = 24.0f;				// Длина прорисовки
	const float Step = 0.01f;				// Шаг луча
	float Speed = 3.5f;					// Скорость игрока
	float PosX = 29.0f;					// Координаты игрока по X
	float PosY = 2.0f;					// Координаты игрока по Y
	float PosA = 0.0f;					// Начальный поворот взгляда игрока
	bool SlabCheck = false;					// Проверка выделяющая полублок из стен
	bool PotionNotEnded = true;				// Проверка на наличие зелья в месте спавна
	int PotionBag = 0;					// Инвентарь
	float PotionEffect = 25.0f;				// Эффект зелья
	Player.MaxHealth = 100.0f;				// Максисмальное здоровье игрока
	Player.Health = 100.0f; 				// Здоровье игрока
	Player.Attack = 6.0f;					// Урон наносимый игроком
	Player.Damage = 0.0f;					// Урон полученный игроком
	Player.IFX = X / 320;					// Прибавка для панели здоровья по X
	Player.IFY = Y / 113.5;					// Прибавка для панели здоровья по Y
	Bat.MaxHealth = 30.0f;					// Максисмальное здоровье врага: Bat
	Bat.Health = 30.0f;					// Здоровье врага: Bat
	Bat.Attack = 2.0f;					// Урон наносимый врагом: Bat
	Bat.Damage = 0.0f;					// Урон полученный врага: Bat
	Bat.IFX = X / 320;					// Прибавка для панели здоровья по X
	Bat.IFY = 2 * Player.IFY + Player.Y;			// Прибавка для панели здоровья по Y

	// Запуск музыки
	PlaySoundA((LPCSTR)"C:\\KH_MazEMusic.WAV", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

	// Создание буфера
	wchar_t* Plane = new wchar_t[X * Y];
	HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	DWORD Painted = 0;

	// Чтение карт
	std::wstring WORLD =		readFile("CARDS\\WORLD.txt");
	std::wstring MAP =			readFile("CARDS\\MAP.txt");
	std::wstring BAT =			readFile("CARDS\\BAT.txt");
	std::wstring MENU =			readFile("CARDS\\MENU.txt");
	std::wstring WIN =			readFile("CARDS\\WIN.txt");
	std::wstring LOSE =			readFile("CARDS\\LOSE.txt");
	std::wstring BATDAMAGED =	readFile("CARDS\\BATDAMAGED.txt");
	std::wstring H_POTION =		readFile("CARDS\\H_POTION.txt");
	Player.HealthPanel =		readFile("CARDS\\PLAYERHP.txt");
	Bat.HealthPanel =			readFile("CARDS\\BATHP.txt");

	// Временные точки
	auto Time = std::chrono::system_clock::now();
	auto OldTime = std::chrono::system_clock::now();

	/*ИГРОВОЙ ЦИКЛ*/
	while (true) {

		// Разница во времени на кадр для вычисления модификации скорости движения
		OldTime = std::chrono::system_clock::now();
		std::chrono::duration<float> TimeDif = OldTime - Time;
		Time = OldTime;
		float FPS = TimeDif.count();

		/*УПРАВЛЕНИЕ*/

		// Поворот мышью
		if (GetCursorPos(&p))
			if (p.y > 150 && p.y < 900) {
				if (p.x < 150)						PosA -= (Speed * 0.35f) * FPS;		// Лево резко
				else if (p.x < 450)					PosA -= (Speed * 0.15f) * FPS;		// Лево
				else if (p.x > 1470 && p.x < 1820)	PosA += (Speed * 0.15f) * FPS;				// Право
				else if (p.x > 1770)				PosA += (Speed * 0.35f) * FPS;			// Право резко
			}

		// Поворот стрелочками
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)		// Лево
			PosA -= 0.45f * Speed * FPS;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	// Право
			PosA += 0.45f * Speed * FPS;

		// Управление WASD и коллизии
		if (GetAsyncKeyState('W') & 0x8000) {
			PosX += sinf(PosA) * FPS * Speed * PI / 2;
			PosY += cosf(PosA) * FPS * Speed * PI / 2;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '1' /*|| WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '2'*/) {
				PosX -= sinf(PosA) * FPS * Speed * PI / 2;
				PosY -= cosf(PosA) * FPS * Speed * PI / 2;
			}
		}
		if (GetAsyncKeyState('A') & 0x8000) {
			PosX -= cosf(PosA) * FPS * Speed * PI / 2;
			PosY += sinf(PosA) * FPS * Speed * PI / 2;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '1' /*|| WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '2'*/) {
				PosX += cosf(PosA) * FPS * Speed * PI / 2;
				PosY -= sinf(PosA) * FPS * Speed * PI / 2;
			}
		}
		if (GetAsyncKeyState('S') & 0x8000) {
			PosX -= sinf(PosA) * FPS * Speed * PI / 2;
			PosY -= cosf(PosA) * FPS * Speed * PI / 2;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '1' /*|| WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '2'*/) {
				PosX += sinf(PosA) * FPS * Speed * PI / 2;
				PosY += cosf(PosA) * FPS * Speed * PI / 2;
			}
		}
		if (GetAsyncKeyState('D') & 0x8000) {
			PosX += cosf(PosA) * FPS * Speed * PI / 2;
			PosY -= sinf(PosA) * FPS * Speed * PI / 2;
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '1' /*|| WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '2'*/) {
				PosX -= cosf(PosA) * FPS * Speed * PI / 2;
				PosY += sinf(PosA) * FPS * Speed * PI / 2;
			}
		}

		// Скорость движения в зависимости от нажатия Shift и использования карты
		if (GetAsyncKeyState(VK_TAB) & 0x8000)				Speed = 1.5f;
		else if (GetAsyncKeyState(VK_SHIFT) & 0x8000)		Speed = 7.5f;
		else												Speed = 3.5f;

		// Рестарт
		if (GetAsyncKeyState('R') & 0x8000) {
			PosX = 29.0f;
			PosY = 2.0f;
			PosA = 0.0f;
			Bat.Health = 25.0f;
			Player.Health = 100.0f;
			Bat.Damage= 0.0f;
			Player.Damage= 0.0f;
			PotionBag = 0;
			PotionNotEnded = true;
			PlaySoundA((LPCSTR)"C:\\KH_MazEMusic.WAV", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

		}
		// Выход
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return 0;

		/*МАТЕМАТИКА ДЛЯ ОТОБРАЖЕНИЯ*/
		for (int x = 0; x < X; x++) {
			float AngleBite = (PosA - FOV / 2.0f) + (x / (float)X) * FOV;

			float DistW = 0.0f;				// Дистанция до стены
			float DistS = 0.0f;				// Дистанция до полублока
			bool Wall = false;				// Попадание в стену
			bool Slab = false;				// Попадание в полублок
			bool Fringe = false;				// Попадание в грань
			bool FringeS = false;				// Попадание в грань полублока

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
				else {

					// Проверка на пересечение с блоком
					if (WORLD.c_str()[WorldYI + WorldXI * WSX] == '1') {
						Wall = true;

						/*ШТРИХОВКА РЁБЕР СТЕН*/

						std::vector<std::pair<float, float>> vec;

						// Проверяем кадый угол блока запоминая расстояние и точечное произведение двух лучей
						for (int CoordCornerX = 0; CoordCornerX < 2; CoordCornerX++)
							for (int CoordCornerY = 0; CoordCornerY < 2; CoordCornerY++) {
								float CornerY = WorldYI + CoordCornerY - PosY;
								float CornerX = WorldXI + CoordCornerX - PosX;
								float Dist = sqrt(CornerX * CornerX + CornerY * CornerY);
								float DOT = (UnitX * CornerX / Dist) + (UnitY * CornerY / Dist);
								vec.push_back(std::make_pair(Dist, DOT));
							}

						// Сортируем рёбра от ближайшего до самого дальнего 
						sort(vec.begin(), vec.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// Ближайшие ребра
						float BoundAngle = 0.005;
						if (acos(vec.at(0).second) < BoundAngle) Fringe = true;
						if (acos(vec.at(1).second) < BoundAngle) Fringe = true;
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
					if (WORLD.c_str()[WorldYI + WorldXI * WSX] == '2' || WORLD.c_str()[WorldYI + WorldXI * WSX] == '1') {
						Slab = true;
						if (WORLD.c_str()[WorldYI + WorldXI * WSX] == '2')
							SlabCheck = true;
						/*ШТРИХОВКА РЁБЕР ПОЛУБЛОКОВ*/

						std::vector<std::pair<float, float>> vec;

						// Проверяем кадый угол блока запоминая расстояние и точечное произведение двух лучей
						for (int CoordCornerX = 0; CoordCornerX < 2; CoordCornerX++)
							for (int CoordCornerY = 0; CoordCornerY < 2; CoordCornerY++) {
								float CornerY = WorldYI + CoordCornerY - PosY;
								float CornerX = WorldXI + CoordCornerX - PosX;
								float Dist = sqrt(CornerX * CornerX + CornerY * CornerY);
								float DOT = (UnitX * CornerX / Dist) + (UnitY * CornerY / Dist);
								vec.push_back(std::make_pair(Dist, DOT));
							}

						// Сортируем рёбра от ближайшего до самого дальнего 
						sort(vec.begin(), vec.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// Ближайшие ребра
						float BoundAngle = 0.005;
						if (acos(vec.at(0).second) < BoundAngle) FringeS = true;
						if (acos(vec.at(1).second) < BoundAngle) FringeS = true;
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
			if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '2') {
				while (Sky < SkyS)
				Sky += 1 + Speed * (int)FPS;
			}

			/*ШТРИХОВКА*/
			short ShadeWall= ' ', ShadeSlab = ' ', ShadeFringle = ' ', ShadeClimb = ' ', ShadeStar = ' '; 
			if (randomNumber(-5, 95) > 0)		ShadeStar = ' ';
			else								ShadeStar = 'F';

			if (DistW <= DEPTH / 6.0f)		ShadeFringle = '!';
			else if (DistW < DEPTH / 4.0f)		ShadeFringle = '!';
			else if (DistW < DEPTH / 3.0f)		ShadeFringle = '!';
			else if (DistW < DEPTH)			ShadeFringle = '!';
			else								ShadeFringle = ShadeStar;

			if (DistW <= DEPTH / 6.0f)		ShadeWall = 'O';
			else if (DistW < DEPTH / 4.0f)		ShadeWall = 'Z';
			else if (DistW < DEPTH / 3.0f)		ShadeWall = 'V';
			else if (DistW < DEPTH)			ShadeWall = 'Y';
			else								ShadeWall = ShadeStar;

			if (DistW <= DEPTH / 6.0f)		ShadeClimb = '5';
			else if (DistW < DEPTH / 4.0f)		ShadeClimb = 'S';
			else if (DistW < DEPTH / 3.0f)		ShadeClimb = '4';
			else if (DistW < DEPTH)			ShadeClimb = 'J';
			else					ShadeClimb = ShadeStar;
			if (Fringe)				ShadeClimb = '!';

			if (DistS <= DEPTH / 6.0f)		ShadeSlab = '5';
			else if (DistS < DEPTH / 4.0f)		ShadeSlab = 'S';
			else if (DistS < DEPTH / 3.0f)		ShadeSlab = '4';
			else if (DistS < DEPTH)			ShadeSlab = 'J';
			else					ShadeSlab = ShadeStar;
			if (FringeS)				ShadeSlab = '!';

			for (int y = 0; y < Y; y++) {
				if (y <= Sky) Plane[y * X + x] = ShadeStar;					// Небо
				else if (y > Sky && y <= Y / 2.0)						// Стена
					Plane[y * X + x] = ShadeWall;										
				else if (y < (Y + SpaceF) / 2.0 && y >(Y - SpaceF) / 2.0			// Ребро полублока
					&& WORLD.c_str()[(int)PosY + (int)PosX * WSX] != '2' && SlabCheck)
					Plane[y * X + x] = ShadeFringle;
				else if (y > Y / 2.0 && y <= Ground-1) {					// Полублок
					if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '2')
						Ground = Y - Sky + 2 * Space;
						Plane[y * X + x] = ShadeWall;
					if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] != '2')			
						Plane[y * X + x] = ShadeSlab;
				}
				else if (y > Ground-1 && y <= Ground) Plane[y * X + x] = ShadeFringle;		// Плинтус
				else {																	// Земля
					float DistG = 1.0f - ((y - Y / 2.0f) / (Y / 2.0f));
					if (DistG < 0.49)		ShadeWall = 'G';			// 0.66			
					else if (DistG < 0.66)	ShadeWall = 'C';				// 0.745		
					else if (DistG < 0.745)	ShadeWall = '3';				// 0.82			
					else if (DistG < 0.95)	ShadeWall = 'L';				// 0.95			
					else					ShadeWall= ' ';
					Plane[y * X + x] = ShadeWall;
				}
				if (WORLD.c_str()[(int)PosX * WSX + (int)PosY] == '2') {
					//if (y > Sky && y <= (Y + SpaceF / 2) / 2.0 + Space) Plane[y * X + x] = ShadeWall;								
					//if (y > (Y - SpaceF / 2) / 2.0 + Space && y <= Ground) Plane[y * X + x] = ShadeClimb;							 
					if (y < (Y + SpaceF / 2) / 2.0 + Space && y >(Y - SpaceF / 2) / 2.0 + Space) Plane[y * X + x] = ShadeFringle;	// Ребро полублока
					if (y > (Y + SpaceF / 2) / 2.0 + Space-1 && y <= Ground) Plane[y * X + x] = ShadeClimb;							// Полублок
					if (y > Ground - 1 && y <= Ground) Plane[y * X + x] = ShadeFringle;												// Плинтус
				}
			}
		}
		
		// Отображение карты
		if (GetAsyncKeyState(VK_TAB) & 0x8000){
			drawCenter(Plane, MAP, MSX, MSY);
			Plane[(((int)PosX) * (MSY / WSY) + (Y - MSY) / 2) * X + (int)PosY * (MSX / WSX) + (X - MSX) / 2] = 0x2592;
		}

		/*БОЁВКА*/

		// Зелье

		if ((PosY >= 5.0f) && (PosY <= 8.0f) && (PosX >= 28.0f) && (PosX <= 30.0f) && (PotionBag == 0) && PotionNotEnded) {
			drawCenter(Plane, H_POTION, 226, 125);
			if (GetAsyncKeyState('E') & 0x8000) {
				PotionBag = 1;
				PotionNotEnded = false;
				continue;
			}
			if (GetAsyncKeyState('Q') & 0x8000) {
				PotionNotEnded = false;
				continue;
			}
		}
		if ((GetAsyncKeyState('H') & 0x8000) && (PotionBag != 0)){
			PotionBag -= 1;
			if (Player.Health <= Player.MaxHealth - PotionEffect) Player.Health += PotionEffect;				// Если здоровье с прибавкой зелья не максимально
			else if (Player.Health >= Player.MaxHealth - PotionEffect) Player.Health = Player.Health = Player.MaxHealth;	// Если здоровье с прибавкой зелья больше максимального
		}
		
		// Бой
		if ((PosY >= 28.0f) && (PosY <= 34.0f) && (PosX >= 30.0f) && (Bat.Health != 0.0f) && (Player.Health != 0.0f)) {
			draw( Plane, BAT, X, Y);					// Отображение BAT
			if (Bat.Health != 0) drawHP (Plane, Bat, 4);			// Здоровье BAT
			drawHP (Plane, Player, 1);					// Здоровье игрока
			// Атака
			while (Bat.Health > 0.0f){
				Player.Health -= Bat.Attack;				// BAT атакует	
				if (GetAsyncKeyState('F') & 0x8000)	{		// Игрок атакует
					Bat.Health -= Player.Attack;
					draw (Plane, BATDAMAGED, X, Y);			// BAT ранена
					if (Bat.Health != 0) drawHP (Plane, Bat, 4);	// Здоровье BAT поверх раненой BAT
					drawHP (Plane, Player, 1);			// Здоровье игрока поверх раненой BAT
				}
				Sleep(700);
				if (Bat.Health != 0) break;
			}
		}

		// Проигрыш
		if (Player.Health <= 0.0f) {
			draw(Plane, LOSE, 240, 64);
			Speed = 0.0f;
		}

		// Победа
		if (PosY >= 63.0f) { 
			//Draw(Plane, WIN, 240, 64); 
			Speed = 0.0f;
		}

		// Меню
		//if (PosY == 2.0f && PosX == 29.0f) Draw(Plane, MENU, 240, 64);

		// Здоровье игрока вне боя
		drawHP(Plane, Player,1);

		// Вывод кадра
		Plane[X * Y - 1] = '\0';
		WriteConsoleOutputCharacter(Console, Plane, X * Y, { 0,0 }, &Painted);
	}
	return 0;
}
