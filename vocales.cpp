#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

constexpr const char* reiniciarColor = "\033[0m";
constexpr std::array<const char*, 6> colores{{
    "\033[95m", "\033[96m", "\033[93m",
    "\033[92m", "\033[94m", "\033[91m"
}};

const std::vector<std::string> palabras{
    "abeja", "aguacate", "arcoiris", "avion", "banana", "burbuja",
    "caballo", "camara", "camino", "camiseta", "caracol", "casa",
    "cereza", "chocolate", "cocodrilo", "conejo", "corazon", "cucurucho",
    "delfin", "dinosaurio", "dulce", "elefante", "ensalada", "escalera",
    "esqueleto", "estrella", "flor", "fresa", "gato", "girasol",
    "helicoptero", "helado", "iglesia", "jirafa", "leon", "libro",
    "limon", "luna", "mar", "maravilloso", "mariposa", "manzana",
    "mesa", "mochila", "mono", "murcielago", "murmullo", "musica",
    "naranja", "nube", "oso", "pajaro", "paloma", "pan",
    "papel", "pelota", "perro", "pez", "pirata", "planta",
    "pulpo", "queso", "raton", "reloj", "rio", "serpiente",
    "sol", "sombrero", "susurro", "tomate", "tortuga", "tren",
    "universo", "uva", "vaca", "ventana", "zapato"
};

int contarVocales(const std::string& palabra, char objetivo) {
    int cantidad = 0;
    for (char letra : palabra) {
        if (objetivo == '\0') {
            if (std::string("aeiou").find(letra) != std::string::npos) {
                ++cantidad;
            }
        } else if (letra == objetivo) {
            ++cantidad;
        }
    }
    return cantidad;
}

struct Pregunta {
    std::array<std::string, 3> opciones;
    int respuestaCorrecta;
};

Pregunta crearPregunta(std::mt19937& generador, char objetivo, int minimo) {
    std::vector<std::string> candidatas;
    for (const std::string& palabra : palabras) {
        if (contarVocales(palabra, objetivo) >= minimo) {
            candidatas.push_back(palabra);
        }
    }
    std::shuffle(candidatas.begin(), candidatas.end(), generador);

    for (const std::string& correcta : candidatas) {
        const int cantidadCorrecta = contarVocales(correcta, objetivo);
        std::vector<std::string> distractores;
        for (const std::string& palabra : palabras) {
            if (palabra != correcta &&
                contarVocales(palabra, objetivo) < cantidadCorrecta) {
                distractores.push_back(palabra);
            }
        }

        if (distractores.size() < 2) {
            continue;
        }

        std::shuffle(distractores.begin(), distractores.end(), generador);
        Pregunta pregunta{{correcta, distractores[0], distractores[1]}, 0};
        std::shuffle(pregunta.opciones.begin(), pregunta.opciones.end(), generador);
        for (std::size_t i = 0; i < pregunta.opciones.size(); ++i) {
            if (pregunta.opciones[i] == correcta) {
                pregunta.respuestaCorrecta = static_cast<int>(i);
                break;
            }
        }
        return pregunta;
    }

    throw std::runtime_error("No se pudieron preparar las palabras del nivel.");
}

int leerRespuesta() {
    int respuesta;
    while (!(std::cin >> respuesta)) {
        if (std::cin.eof()) {
            return std::numeric_limits<int>::min();
        }
        std::cout << "Escribe el numero 1, 2 o 3: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (respuesta < 1 || respuesta > 3) {
        std::cout << "Elige una opcion del 1 al 3: ";
        if (!(std::cin >> respuesta)) {
            if (std::cin.eof()) {
                return std::numeric_limits<int>::min();
            }
            std::cout << "Escribe un numero: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            respuesta = 0;
        }
    }
    return respuesta;
}

void prepararConsola() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE salida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (GetConsoleMode(salida, &modo)) {
        SetConsoleMode(salida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

void mostrarConejo() {
    std::cout << colores[0]
              << "        /)  /)\n"
                 "       (  .. )\n"
                 "       ( づ♡と )  ¡Hola!\n"
                 "      /       \\\n"
                 "     (         )\n"
                 "      U-----U\n"
              << reiniciarColor;
}

int main() {
    prepararConsola();
    std::random_device fuenteAleatoria;
    std::mt19937 generador(fuenteAleatoria());
    constexpr std::array<char, 5> vocales{{'a', 'e', 'i', 'o', 'u'}};
    int nivelesSuperados = 0;

    mostrarConejo();
    std::cout << colores[1]
              << "¡Bienvenidas y bienvenidos, niños y niñas!\n"
                 "Soy Conejito y me alegra mucho jugar con ustedes. 🐰🌈\n"
                 "En cada nivel, elige la palabra que tenga más veces la vocal indicada.\n"
                 "En los últimos cinco niveles, busca la palabra con más vocales en total.\n"
                 "Tienes hasta 3 intentos por nivel. ¡Tú puedes! 🍓⭐\n\n"
              << reiniciarColor;

    for (int nivel = 0; nivel < 10; ++nivel) {
        const bool general = nivel >= 5;
        const char objetivo = general ? '\0' : vocales[static_cast<std::size_t>(nivel)];
        const int minimo = general ? (nivel < 7 ? 2 : (nivel < 9 ? 3 : 4)) : 2;
        const Pregunta pregunta = crearPregunta(generador, objetivo, minimo);

        std::cout << colores[static_cast<std::size_t>(nivel) % colores.size()]
                  << "\n💖 NIVEL " << (nivel + 1) << " DE 10 💖\n";
        if (general) {
            std::cout << "¿Qué palabra tiene más vocales en total? 🔎\n";
        } else {
            std::cout << "¿Qué palabra tiene más veces la vocal '"
                      << objetivo << "'? 🐰\n";
        }
        std::cout << reiniciarColor;

        for (std::size_t i = 0; i < pregunta.opciones.size(); ++i) {
            std::cout << colores[(i + static_cast<std::size_t>(nivel)) % colores.size()]
                      << "  " << (i + 1) << ") " << pregunta.opciones[i]
                      << reiniciarColor << '\n';
        }

        bool acerto = false;
        for (int intento = 1; intento <= 3; ++intento) {
            std::cout << "Tu respuesta: ";
            const int respuesta = leerRespuesta();
            if (respuesta == std::numeric_limits<int>::min()) {
                std::cout << "\n¡Gracias por jugar! El conejito te manda un abrazo. 🐰💖\n";
                return 0;
            }

            if (respuesta - 1 == pregunta.respuestaCorrecta) {
                acerto = true;
                ++nivelesSuperados;
                std::cout << colores[3]
                          << "¡Correcto! ¡Eres genial! 🌟🐇🎉\n"
                          << reiniciarColor;
                break;
            }

            if (intento < 3) {
                std::cout << colores[5]
                          << "¡Casi! Prueba otra vez, tú puedes. 🍀\n"
                          << reiniciarColor;
            }
        }

        if (!acerto) {
            std::cout << colores[2]
                      << "La respuesta era: "
                      << pregunta.opciones[static_cast<std::size_t>(pregunta.respuestaCorrecta)]
                      << ". ¡Cada intento te ayuda a aprender! 💛\n"
                      << reiniciarColor;
        }
    }

    std::cout << colores[0]
              << "\n🎊 ¡Terminaste los 10 niveles! 🎊\n"
              << "Superaste " << nivelesSuperados << " nivel(es). ¡Conejito está muy orgulloso de ti! 🐰💖\n"
              << "¡Sigue leyendo y jugando con las vocales! 🌈🦋⭐\n"
              << reiniciarColor;
    return 0;
}
