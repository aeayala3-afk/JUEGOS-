#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

constexpr const char* reiniciarColor = "\033[0m";

enum class Color {
    rojo,
    amarillo,
    azul,
    verde,
    naranja,
    morado,
    rosa,
    blanco,
    negro,
    cafe
};

struct InfoColor {
    const char* nombre;
    const char* codigo;
};

constexpr std::array<InfoColor, 10> colores{{
    {"rojo", "\033[91m"},
    {"amarillo", "\033[93m"},
    {"azul", "\033[94m"},
    {"verde", "\033[92m"},
    {"naranja", "\033[38;5;208m"},
    {"morado", "\033[95m"},
    {"rosa", "\033[38;5;205m"},
    {"blanco", "\033[97m"},
    {"negro", "\033[90m"},
    {"cafe", "\033[33m"}
}};

struct Objeto {
    const char* nombre;
    const char* dibujo;
    Color colorReal;
};

constexpr std::array<Objeto, 30> objetos{{
    {"manzana", "🍎", Color::rojo},
    {"fresa", "🍓", Color::rojo},
    {"tomate", "🍅", Color::rojo},
    {"cereza", "🍒", Color::rojo},
    {"banana", "🍌", Color::amarillo},
    {"sol", "☀️", Color::amarillo},
    {"limon", "🍋", Color::amarillo},
    {"patito", "🐤", Color::amarillo},
    {"cielo", "🌤️", Color::azul},
    {"mar", "🌊", Color::azul},
    {"pajaro", "🐦", Color::azul},
    {"pez", "🐟", Color::azul},
    {"rana", "🐸", Color::verde},
    {"hoja", "🍃", Color::verde},
    {"brocoli", "🥦", Color::verde},
    {"pera", "🍐", Color::verde},
    {"naranja", "🍊", Color::naranja},
    {"zanahoria", "🥕", Color::naranja},
    {"calabaza", "🎃", Color::naranja},
    {"uvas", "🍇", Color::morado},
    {"berenjena", "🍆", Color::morado},
    {"ciruela", "🫐", Color::morado},
    {"flamenco", "🦩", Color::rosa},
    {"cerdito", "🐷", Color::rosa},
    {"flor", "🌸", Color::rosa},
    {"nube", "☁️", Color::blanco},
    {"leche", "🥛", Color::blanco},
    {"conejo", "🐇", Color::blanco},
    {"gato", "🐈‍⬛", Color::negro},
    {"chocolate", "🍫", Color::cafe}
}};

struct Opcion {
    std::size_t objeto;
    Color colorMostrado;
};

struct Pregunta {
    Color objetivo;
    std::array<Opcion, 3> opciones;
    std::size_t respuestaCorrecta;
};

const InfoColor& informacion(Color color) {
    return colores[static_cast<std::size_t>(color)];
}

Pregunta crearPregunta(std::mt19937& generador, Color objetivo) {
    std::vector<std::size_t> correctos;
    std::vector<std::size_t> distractores;
    for (std::size_t i = 0; i < objetos.size(); ++i) {
        if (objetos[i].colorReal == objetivo) {
            correctos.push_back(i);
        } else {
            distractores.push_back(i);
        }
    }

    std::shuffle(correctos.begin(), correctos.end(), generador);
    std::shuffle(distractores.begin(), distractores.end(), generador);

    const std::size_t objetoCorrecto = correctos.front();
    Pregunta pregunta{
        objetivo,
        {{{objetoCorrecto, objetivo},
          {distractores[0], objetivo},
          {distractores[1], objetos[distractores[1]].colorReal}}},
        0
    };
    std::shuffle(pregunta.opciones.begin(), pregunta.opciones.end(), generador);

    for (std::size_t i = 0; i < pregunta.opciones.size(); ++i) {
        if (pregunta.opciones[i].objeto == objetoCorrecto) {
            pregunta.respuestaCorrecta = i;
            break;
        }
    }
    return pregunta;
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

void mostrarOpcion(const Opcion& opcion, std::size_t numero) {
    const Objeto& objeto = objetos[opcion.objeto];
    const InfoColor& color = informacion(opcion.colorMostrado);
    std::cout << "  " << (numero + 1) << ") " << objeto.dibujo << ' '
              << objeto.nombre << " de color " << color.codigo << color.nombre
              << reiniciarColor << "  " << color.codigo << "■" << reiniciarColor << '\n';
}

bool jugar(std::mt19937& generador) {
    std::array<Color, 10> niveles{{
        Color::rojo, Color::amarillo, Color::azul, Color::verde,
        Color::naranja, Color::morado, Color::rosa, Color::blanco,
        Color::negro, Color::cafe
    }};
    std::shuffle(niveles.begin(), niveles.end(), generador);
    int nivelesSuperados = 0;

    std::cout << colores[5].codigo
              << "\n      .-\"\"\"-.\n"
                 "     / .===. \\\n"
                 "    / /6 6\\ \\\n"
                 "    |/  ^  \\|\n"
                 "     \\ '-' /\n"
                 "      '---'\n"
              << reiniciarColor
              << "¡Hola, cerebritos! Vamos a descubrir los colores.\n"
                 "Elige el objeto que de verdad tiene el color indicado.\n"
                 "¡Ojo! Algunas opciones tienen colores cambiados para poner a prueba tu atencion.\n"
                 "Tienes 3 intentos por nivel.\n";

    for (std::size_t nivel = 0; nivel < niveles.size(); ++nivel) {
        const Pregunta pregunta = crearPregunta(generador, niveles[nivel]);
        const InfoColor& objetivo = informacion(pregunta.objetivo);
        std::cout << '\n' << objetivo.codigo << "🎨 NIVEL " << (nivel + 1)
                  << " DE " << niveles.size() << " 🎨\n"
                  << "¿Que cosa suele ser de color " << objetivo.codigo
                  << objetivo.nombre << reiniciarColor << "?\n";

        for (std::size_t i = 0; i < pregunta.opciones.size(); ++i) {
            mostrarOpcion(pregunta.opciones[i], i);
        }

        bool acerto = false;
        for (int intento = 1; intento <= 3; ++intento) {
            std::cout << "Tu respuesta: ";
            const int respuesta = leerRespuesta();
            if (respuesta == std::numeric_limits<int>::min()) {
                std::cout << "\n¡Gracias por jugar! ¡Hasta pronto!\n";
                return false;
            }

            if (static_cast<std::size_t>(respuesta - 1) == pregunta.respuestaCorrecta) {
                acerto = true;
                ++nivelesSuperados;
                std::cout << colores[3].codigo << "¡Correcto! ¡Buen ojo! ⭐\n"
                          << reiniciarColor;
                break;
            }

            if (intento < 3) {
                std::cout << colores[0].codigo << "¡Casi! Mira bien el objeto y su color.\n"
                          << reiniciarColor;
            }
        }

        if (!acerto) {
            const Opcion& correcta = pregunta.opciones[pregunta.respuestaCorrecta];
            const Objeto& objetoCorrecto = objetos[correcta.objeto];
            std::cout << "La respuesta era: " << objetoCorrecto.dibujo << ' '
                      << objetoCorrecto.nombre << " de color " << objetivo.codigo
                      << objetivo.nombre << reiniciarColor << ".\n";
        }
    }

    std::cout << colores[5].codigo << "\n🎉 ¡Terminaste los 10 niveles! 🎉\n"
              << "Respondiste correctamente " << nivelesSuperados << " de 10.\n"
              << "¡Sigue entrenando ese cerebrito! 🌈⭐\n" << reiniciarColor;
    return true;
}

int main() {
    prepararConsola();
    std::random_device fuenteAleatoria;
    std::mt19937 generador(fuenteAleatoria());
    std::string respuesta;

    do {
        jugar(generador);
        if (std::cin.eof()) {
            break;
        }

        std::cout << "\n¿Quieres jugar otra vez? (si/no): ";
        std::cin >> respuesta;
        while (respuesta != "si" && respuesta != "SI" && respuesta != "Si" &&
               respuesta != "no" && respuesta != "NO" && respuesta != "No") {
            std::cout << "Escribe si o no: ";
            std::cin >> respuesta;
        }
    } while (respuesta == "si" || respuesta == "SI" || respuesta == "Si");

    std::cout << "¡Gracias por jugar a Cerebritos! 🧠🌈\n";
    return 0;
}