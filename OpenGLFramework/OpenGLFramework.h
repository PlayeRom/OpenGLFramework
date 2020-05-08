#pragma once

#include "resource.h"

/** \mainpage
 *
 * \section intro_sec Wstêp
 *
 * OpenGLFramework jest to prosty szablon, na podstawie którego mo¿emy budowaæ aplikacje (w tym gry) oparte
 * na bibliotece OpenGL. I takie te¿ by³o jego g³ówne za³o¿enie. OpenGLFramework zawiera wiêc podstawowe
 * funkcje zwi¹zane chyba z ka¿d¹ gr¹. Ca³¹ resztê, tj. funkcjonalnoœæ, mechanizm gry nale¿y ju¿ wykonaæ we
 * w³asnym zakresie, dok³adaj¹c do opisywanego tu szablonu w³asne klasy/modu³y. \n
 * Na stan obecny OpenGLFramework zawiera:
 * - wczytywanie tekstur (nearest, linear oraz mipmap) z plików BMP oraz TGA,
 * - multiteksturing,
 * - obs³uga dŸwiêku za pomoc¹ biblioteki FMOD oraz OpenAL,
 * - wyœwietlanie tekstu – a¿ piêæ mo¿liwoœæ,
 * - obliczanie i wyœwietlanie FPS (iloœæ generowanych klatek na sekundê),
 * - kontrola prêdkoœci animacji, dziêki czemu animacje wyœwietlane bêd¹ z prêdkoœci¹ niezale¿n¹ od FPS,
 * - prosty profiler do sprawdzania czasu wykonania siê ró¿nych funkcji,
 * - konsola do wyœwietlania i wprowadzania komunikatów,
 * - wspieranie UNICODE,
 * - wspieranie wielojêzycznoœci,
 * - wspieranie oœwietlania i materia³ów,
 * - pe³noekranowy antyaliasing,
 * - logger,
 * - tryb 2D oraz 3D z mo¿liwoœci¹ prze³¹czania siê,
 * - wykorzystanie VertexArrays,
 * - wykorzystanie Vertex Buffer Object,
 * - w³asny format pliku 3DObj dla modeli 3D,
 * - w³¹czanie / wy³¹czanie synchronizacji pionowej,
 * - emboss bump mapping (mapowanie wybojów),
 * - prosty system cz¹steczkowy,
 * - rysowanie billboard'ów (sprite'ów zawsze zwróconych przodem do widza),
 * - mo¿liwoœæ zmiany, np. rozdzielczoœci, g³êbi kolorów, próbkowania dla antyaliasingu, itp. bez utraty 
aktualnego stanu gry,
 * - mo¿liwoœæ tworzenia cieni w oparciu o stencil buffer.
 *
 * \section library_sec Wykorzystywane biblioteki.
 * 
 * OpenGLFramework wykorzystuje takie biblioteki jak SDL_ttf, FMOD, OpenAL oraz STL no i oczywiœcie OpenGL.
 * Oznacza to, ¿e aby skompilowaæ projekt nale¿y posiadaæ biblioteki SDL, SDL_ttf, FMOD oraz OpenAL – mo¿emy
 * pobraæ je z internetu (pozosta³e powinniœmy ju¿ mieæ). Oczywiœcie musimy w Visual C++ wskazaæ œcie¿ki do
 * folderów include oraz lib – dla ka¿dej z biblioteki z osobna. Do g³ównego katalogu projektu nale¿y tak¿e
 * skopiowaæ pliki DLL i s¹ to: fmod.dll (o ile zamierzamy korzystaæ z biblioteki FMOD a nie z OpenAL), alut.dll,
 * OpenAL32.dll, ogg.dll, vorbis.dll i vorbisfile.dll (o ile zamierzamy korzystaæ z biblioteki OpenAL a nie FMOD),
 * SDL.dll oraz SDL_ttf.dll.
 */