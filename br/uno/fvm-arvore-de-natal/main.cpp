#include <Arduino.h>
#include <string.h>

#define OR  ||
#define AND &&

const u8 buzzerPin = 11;
const u8 buttonPin = A5;
const bool debug = false;

const char song_Unknown[] = "T100 ||:"
	"8b b 4E 8E Eb 4E | 8Eb E Gb Ab A Gb 4Ab |"
	"8Ab A 4B ^ 8Db a ab e | 4gb 2e 4- v :||";

const char song_JoyToTheWorld[] = "T100 ||:"
	"4D 8C#. 16b 4a. 8g | 4f# e d. 8a | 4b. 8b 4C#. 8C# | 2D. 8- D  |"
	"8D C# b a a. 16g 8f# D | D C# b a a. 16g 8f# f# |"
	"f# f# f# 16f# g 4a. 16g f# | 8e e e 16e f# 4g. 16f# e |"
	"8d 4D 8b a. 16g 8f# g | 4f# e 2d :||";

const char song_JBRock[] = "T138 "
	"8C C 4C 8b b 4b | 8a b a e_2e | 8a b 4a e g |"
	"8a b 4a 2f | 8d 4e 8f g 4a 8g | 8d e 4f 2g  |"
	"4- 8a g a g 4a | a 8d d_ 2d |"
	
	"8C C 4C 8b b 4b | 8a b a e_2e | 8a b 4a e g |"
	"8a b 4a 2f | 8d 4e 8f g 4a 8g | 8d e 4f 2g  |"
	"4- 8a a b 4g 8C_ | 2C. 8C C |";

const char song_SilentNight[] = "T90 "
	"4g. 5a 4g | 2e. | 4g. 8a 4g | 2e. |"
	"2D     4D | 2b. | 2C     4C | 2g. |"
	"2a     4a | C. 8b 4a |"
	"4g. 8a 4g | 2e. | 2a 4a | 4C. 8b 4a |"
	"4g. 8a 4g | 2e. | 2D 4D | 4F. 8D 4b |"
	"2C.       | 2E. | 4C g e | g. 8f 4d |"
	"2c.";

#define NUM_SONGS (sizeof(songs)/sizeof(*songs))
const char* songs[] = {
	song_Unknown, song_JoyToTheWorld,
	song_JBRock, song_SilentNight
};

void ledEffect();

//====================================================================
// TOCADOR DE MÚSICAS
//====================================================================

int tones[12]; // frequências para uma oitava

void generateTones() {
	const float a = 1.059463094359;
	float frq = 3520 * a*a*a;	// começa no C8
	for (int i=0; i<12; i++) {	// gera até o B8
		tones[i] = (int)round(frq);
		// Serial.println(frq);
		frq *= a;
	}
}

void playNote(u8 num, u8 octave, u32 duration, bool legato)
{
	static int prevFrq = 0;
	static bool playLeg = false;
	
	if (num >= 12 || octave > 8) {
		noTone(buzzerPin);
		delay(duration);
		return;
	}
	// Divide a frequência por 2 até chegar na oitava desejada
	int frq = tones[num] >> (8-octave);
	
	if (frq != prevFrq OR !playLeg)
		tone(buzzerPin, frq);
		
	if (legato) {
		delay(duration);
		playLeg = true;
		prevFrq = frq;
	}
	else {
		int live = 95 * duration / 100;
		delay(live);
		noTone(buzzerPin);
		delay(duration-live);
		playLeg = false;
	}
}

bool isOrd(char c) { return (c>='0' AND c<='9'); }
bool isLow(char c) {	return (c>='a' AND c<='g'); }
bool isUpp(char c) {	return (c>='A' AND c<='G'); }
bool isAlp(char c) { return isLow(c) OR isUpp(c); }

bool contains(const char* str, char c) {
	while (*str != 0)
		if (*str++ == c)
			return true;
	return false;
}

bool containsA(const char* str) {
	while (*str != 0)
		if (isAlp(*str++))
			return true;
	return false;
}

const char
	cOctUp  = '^',
	cOctDn  = 'v',
	cPause  = '-',
	cLegato = '_';

#define BUFFER_LENGTH 8

int capture(char* buffer, const char* str,
	bool num, bool alp, const char* other="")
{
	int i;
	for (i=0; i<BUFFER_LENGTH-1; i++) {
		char c = str[i];
		if (c == ' ') break;
		if (contains(other, c)) {
		}
		else if (isOrd(c)) {
			if (!num) break;
		}
		else if (isAlp(c)) {
			if (!alp) break;
		}
		else break;
		buffer[i] = c;
	}
	buffer[i] = 0;
	return i;
}

char* nextToken(const char*& ptr)
{
	static char buffer[BUFFER_LENGTH];
	int l = 0;
	
	while (*ptr == ' ') ptr++;			// trim white spaces
	if (*ptr == 0) return nullptr;	// no more tokens
	
	char c = *ptr;
	if (isOrd(c)) {
		l = capture(buffer, ptr, true, false);
	}
	else if (isAlp(c)) {
		l = capture(buffer, ptr, false, true, "#b._");
	}
	else if (c == 'T') {
		l = capture(buffer, ptr, true, false, "T");
	}
	else if (c == ':' OR c == '|') {
		// Several types of division bars
		l = capture(buffer, ptr, true, true, "|:.");
		if (l >= 3 AND buffer[2] == ':') {
			buffer[3] = 0;
			l = 3;
		}
		else if (containsA(buffer) OR buffer[l-1] != '.') {
			l = capture(buffer, ptr, false, false, "|:");
		}
	}
	else {
		// Capture a single character
		buffer[0] = c;
		buffer[1] = 0;
		l = 1;
	}
	
	if (l == 0) return nullptr;	// couldn't find any token
	
	ptr += l;
	return buffer;		// return address to the token's buffer
}

void playSong(const char* song, const char* until=nullptr)
{
	static int octave = 5;
	static int whole = 2500;
	static int duration = whole;
	// static bool legato = false;
	static int repNum = 0;
	const char* ptr = song;
	
	while (true) {
		// Captura o próximo token
		char* tok = nextToken(ptr);
		if (tok == nullptr) break;
		if (until != nullptr) {
			if (ptr >= until)
				break;
		}
		
		// Interpreta o token
		char c = tok[0];
		if (isOrd(c)) {
			// Muda a duração das notas
			int num = atoi(tok);
			duration = whole/num;
		}
		else if (c == 'T') {
			// Muda o tempo da música
			int n = atoi(tok+1);
			if (n>=5 AND n<=180) {
				// Transforma em duração de uma nota cheia
				whole = 4*60000/n;
			}
		}
		
		else if (c == cOctUp)  { if (octave < 8) octave++; }
		else if (c == cOctDn)  { if (octave > 0) octave--; }
		else if (c == cPause)  {
			playNote(100, 0, duration, false);
			// delay(duration);
		}
		// else if (c == cLegato) { legato = true; }
		
		else if (c == '|') {
			if (tok[1] == '|' AND tok[2] == ':') {
				// Repetição de uma parte da música
				for (const char* p1 = ptr; *p1 != 0; p1++) {
					// Encontrou o fim da repetição?
					if (strncmp(p1, ":||", 3) == 0) {
						for (repNum = 1; repNum < 3; repNum++) {
							const char* p0 = ptr;
							playSong(p0, p1);
						}
						ptr = p1;
						nextToken(ptr);
						break;
					}
				}
			}
			else if (isOrd(tok[1])) {
				int n = tok[1]-'0';
				if (n != repNum) {
					// Avança para o próximo compasso
					while (*ptr!=0 AND ptr!=until AND *ptr!='|')
						ptr++;
				}
			}
		}
		else if (c == ':') {
		}
		else {
			// Toca uma nota
			int n = -1;
			int o = octave;
			int d = duration;
			if (c>='A' AND c<='G') { n = c-'A'; o++; }
			else if (c>='a' AND c<='g') { n = c-'a'; }
			
			if (n >= 0) {
				// Procura o índice do semitom
				const int map[] = {9,11,0,2,4,5,7};
				n = map[n];
				
				// Procura modificador de semi-tons
				c = tok[1];
				if (c == 'b') { n--; c=tok[2]; }
				else if (c == '#') { n++; c=tok[2]; }
				while (n < 0) {n += 12; o--;}
				while (n >= 12) {n -= 12; o++;}
				
				// Procura extensão de duração
				if (c == '.') { d += d/2; c=tok[3]; }
				
				ledEffect();
				playNote(n, o, d, c=='_');
			}
		}
		if (debug) Serial.println(tok);
		// cout << '(' << tok << ") ";
	}
	noTone(buzzerPin);
}

//====================================================================
// EFEITOS DE LEDS
//====================================================================

const u8 bluLeds[] = {0, 1, 2, 3};
const u8 grnLeds[] = {4, 5, 6, 7};
const u8 redLeds[] = {13, 12, 8, 10};
const u8 whtLeds[] = {A0, A1, A2, A3, A4};
const u8 allLeds[] = {0,1,2,3,4,5,6,7,8,10,12,13,A0,A1,A2,A3,A4};

void ledGroup(u8 group, bool state) {
	const u8* leds;
	switch (group) {
		case 0: leds = bluLeds; break;
		case 1: leds = grnLeds; break;
		case 2: leds = redLeds; break;
		
		case 3:
			for (u8 led: whtLeds)
				digitalWrite(led, state);
			return;
			
		default: // acende todos os LEDs
			for (u8 led: allLeds)
				digitalWrite(led, state);
			return;
	}
	for (int i = 0; i < 4; i++)
		digitalWrite(*leds++, state);
}

void ledStair(u8 stage, bool on) {
	if (stage < 4) {
		digitalWrite(bluLeds[stage], on);
		digitalWrite(grnLeds[stage], on);
		digitalWrite(redLeds[stage], on);
	}
	else switch (stage) {
		case 4:
			digitalWrite(whtLeds[0], on);
			digitalWrite(whtLeds[4], on);
			break;
		
		case 5:
			digitalWrite(whtLeds[1], on);
			digitalWrite(whtLeds[3], on);
			break;
		
		case 6:
			digitalWrite(whtLeds[2], on);
			break;
	}
}

/// Ilumina de forma ascendente.
bool risingLeds(u8 step)
{
	if (step < 8)
		ledStair(step, true);		// acende um a um
	
	else if (step < 16)
		ledStair(step-8, false);	// apaga um a um
	
	else return true;					// muda efeito
	return false;
}

/// Ilumina de forma descendente.
bool fallingLeds(u8 step)
{
	if (step < 8)
		ledStair(6-step, true);		// acende um a um
		
	else if (step < 16)
		ledStair(14-step, false);	// apaga um a um
	
	else return true;					// muda o efeito
	return false;
}

/// Ilumina cores individuais
bool colorLeds(u8 step) {
	if (step < 9) {
		switch (step%3) {
			case 0: ledGroup(0, true); ledGroup(2, false); break;
			case 1: ledGroup(1, true); ledGroup(0, false); break;
			case 2: ledGroup(2, true); ledGroup(1, false); break;
		}
	}
	else if (step == 9) {
		ledGroup(100, false);
	}
	else return true;
	return false;
}

/// Pisca todos os leds.
/// Termina com todos apagados.
bool blinkLeds(u8 step) {	
	if (step < 8) {
		ledGroup(100, (step+1)&1);
		return false;
	}
	return true;
}

/// Controle principal de efeitos
void ledEffect() {
	static u8 fx = 0;
	static u8 step = 0;
	bool end;
	do {
		switch (fx) {
			case 0: end = risingLeds(step);	break;
			case 1: end = colorLeds(step);	break;
			case 2: end = fallingLeds(step);	break;
			case 3: end = blinkLeds(step);	break;
		}
		if (end) {
			// Próximo efeito
			fx = (fx+1) % 4;
			step = 0;
		}
	} while (end);
	step++;
}

//====================================================================
// ENTRADA DO PROGRAMA
//====================================================================

void setup()
{
	if (debug) Serial.begin(9600);
	generateTones();
	for (u8 pin : allLeds) pinMode(pin, OUTPUT);
	pinMode(buttonPin, INPUT_PULLUP);
	ledEffect();
}

void loop()
{
	static int songTimer = 0;
	static u8 stopTimer = 0;
	static u8 currSong = 0;
	static u8 ticks = 0;
	
	bool play = false;
	
	// Toca música quando o botão for pressionado
	if (digitalRead(buttonPin) == LOW) {
		while (digitalRead(buttonPin) == LOW)
			delay(100);
		play = true;
	}
	
	delay(100); // intervalo de "1 tick"
	
	// Muda efeito dos LEDs
	if (++ticks >= 10) {
		ticks = 0;
		// Toca uma música a cada 15 minutos
		if (++songTimer >= 15*60) {
			songTimer = 0;
			// Mas para depois de 2 horas
			if (stopTimer < 8) {
				stopTimer++;
				play = true;
			}
		}
		else ledEffect();
	}
	
	if (play) {
		// Toca uma música
		playSong(songs[currSong]);
		if (++currSong == NUM_SONGS)
			currSong = 0;
		play = false;
	}
	
}