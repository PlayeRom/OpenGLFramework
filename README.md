# OpenGLFramework

OpenGLFramework jest to prosty szablon, na podstawie którego możemy budować aplikacje (w tym gry) oparte na bibliotece OpenGL. I takie też było jego główne założenie. OpenGLFramework zawiera więc podstawowe funkcje związane chyba z każdą grą. Całą resztę, tj. funkcjonalność, mechanizm gry należy już wykonać we własnym zakresie, dokładając do opisywanego tu szablonu własne klasy/moduły.
Na stan obecny OpenGLFramework zawiera:

* wczytywanie tekstur (nearest, linear oraz mipmap) z plików BMP oraz TGA,
* multiteksturing,
* obsługa dźwięku za pomocą biblioteki FMOD oraz OpenAL,
* wyświetlanie tekstu – pięć możliwość,
* obliczanie i wyświetlanie FPS (ilość generowanych klatek na sekundę),
* kontrola prędkości animacji, dzięki czemu animacje wyświetlane będą z prędkością niezależną od FPS,
* prosty profiler do sprawdzania czasu wykonania się różnych funkcji,
* konsola do wyświetlania i wprowadzania komunikatów,
* wspieranie UNICODE,
* wspieranie wielojęzyczności,
* wspieranie oświetlania i materiałów,
* pełnoekranowy antyaliasing,
* logger,
* tryb 2D oraz 3D z możliwością przełączania się,
* wykorzystanie VertexArrays,
* wykorzystanie Vertex Buffer Object,
* własny format pliku 3DObj dla modeli 3D,
* włączanie / wyłączanie synchronizacji pionowej,
* emboss bump mapping (mapowanie wybojów),
* prosty system cząsteczkowy,
* rysowanie billboard'ów (sprite'ów zawsze zwróconych przodem do widza),
* możliwość zmiany, np. rozdzielczości, głębi kolorów, próbkowania dla antyaliasingu, itp. bez utraty aktualnego stanu gry,
* możliwość tworzenia cieni metodą shadow volume.