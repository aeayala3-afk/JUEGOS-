#include <array>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

struct Planeta {
    const char* nombre;
    const char* color;
    const char* dibujo;
};

struct Objeto {
    const char* nombre;
    const char* nombreSingular;
    const char* dibujo;
};

constexpr std::array<Planeta, 9> planetas{{
    {"Neptuno", "\033[94m", "       .-~~~~-.       \n     .'        '.     \n    /            \\    \n   |   NEPTUNO    |   \n    \\            /    \n     '.        .'     \n       '-.__.-'       "},
    {"Urano", "\033[96m", "       .-~~~~-.       \n     .'        '.     \n    /            \\    \n   |    URANO     |   \n    \\            /    \n     '.        .'     \n       '-.__.-'       "},
    {"Saturno", "\033[93m", "        .-~~~~-.        \n  ====='  SATURNO '=====\n        '-.____.-'        "},
    {"Jupiter", "\033[33m", "       .-~~~~-.       \n     .'  JUPITER '.    \n    /   (  o  )   \\   \n   |              |   \n    \\            /    \n     '.        .'     \n       '-.__.-'       "},
    {"Marte", "\033[91m", "       .-~~~~-.       \n     .'  MARTE   '.    \n    /              \\  \n   |       *        |  \n    \\              /  \n     '.          .'   \n       '-.____.-'      "},
    {"Tierra", "\033[92m", "       .-~~~~-.       \n     .'  TIERRA  '.    \n    /    _.._      \\  \n   |   .'    '.     |  \n   |   '.____.'     |  \n    \\              /  \n     '-.________.-'    "},
    {"Venus", "\033[95m", "       .-~~~~-.       \n     .'  VENUS   '.    \n    /              \\  \n   |     .----.     |  \n    \\              /  \n     '.          .'   \n       '-.____.-'      "},
    {"Mercurio", "\033[37m", "       .-~~~~-.       \n     .' MERCURIO '.    \n    /   .-..-.     \\  \n   |   /      \\    |  \n    \\   '-..-'    /   \n     '.          .'   \n       '-.____.-'      "},
    {"Sol", "\033[93m", "          \\   |   /          \n        .---\\  |  /---.        \n       /     \\ | /     \\       \n      |       \\|/       |      \n  ----|      (SOL)      |----  \n      |       /|\\       |      \n       \\     / | \\     /       \n        '---/  |  \\---'        \n          /   |   \\            "}
}};

constexpr std::array<Objeto, 9> objetos{{
    {"manzanas", "manzana", "🍎"},
    {"peras", "pera", "🍐"},
    {"uvas", "uva", "🍇"},
    {"carros", "carro", "🚗"},
    {"estrellas", "estrella", "⭐"},
    {"peces", "pez", "🐟"},
    {"flores", "flor", "🌼"},
    {"globos", "globo", "🎈"},
    {"pizzas", "pizza", "🍕"}
}};

constexpr std::array<const char*, 9> chistes{{
    "Que le dijo una manzana a otra? Nos vemos en el arbol.",
    "Que hace una pera esperando? Espera.",
    "Que uva saluda primero? La uva que dice hola.",
    "Por que el carro fue a la escuela? Para aprender a conducir sus ideas.",
    "Que estrella trabaja en una panaderia? La estrella de masa.",
    "Que hace un pez en una computadora? Nada por la red.",
    "Por que la flor sonrio? Porque le echaron un buen chiste.",
    "Que le dijo un globo a otro? No te infles, todo saldra bien.",
    "Que pizza nunca llega tarde? La que siempre esta a tiempo de horno."
}};

constexpr const char* reiniciarColor = "\033[0m";

void tocarNota(int frecuencia, int duracionMs) {
#ifdef _WIN32
    Beep(frecuencia, duracionMs);
#else
    (void)frecuencia;
    (void)duracionMs;
#endif
}

void tocarInicio() {
    const int notas[] = {196, 262, 330, 392};
    for (int nota : notas) {
        tocarNota(nota, 120);
    }
}

void tocarCorrecto() {
    const int notas[] = {392, 523, 659, 783};
    for (int nota : notas) {
        tocarNota(nota, 140);
    }
}

void tocarError() {
    const int notas[] = {220, 165, 131};
    for (int nota : notas) {
        tocarNota(nota, 180);
    }
}

void tocarVictoria() {
    const int notas[] = {262, 330, 392, 523, 659, 783};
    for (int nota : notas) {
        tocarNota(nota, 160);
    }
}

void tocarDerrota() {
    const int notas[] = {440, 330, 220, 165};
    for (int nota : notas) {
        tocarNota(nota, 200);
    }
}

void mostrarPlaneta(const Planeta& planeta) {
    std::cout << planeta.color << planeta.dibujo << reiniciarColor << "\n";
    std::cout << "Has llegado a " << planeta.nombre << ".\n";
}

void mostrarObjetos(const Objeto& objeto, int cantidad) {
    for (int i = 0; i < cantidad; ++i) {
        std::cout << objeto.dibujo << ' ';
    }
}

void mostrarPollitoYChiste(const char* chiste) {
    std::cout << "\n"
                 "       🐤\n"
                 "   ,~.\n"
                 "  (o o)  PIO!\n"
                 "  / V \\\n"
                 " //   \\\\\n"
                 "Pollito dice: "
              << chiste << "\n\n";
}

int leerRespuesta() {
    int respuesta;
    while (!(std::cin >> respuesta)) {
        if (std::cin.eof()) {
            return std::numeric_limits<int>::min();
        }
        std::cout << "Escribe un numero entero: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return respuesta;
}

bool jugar(std::mt19937& generador) {
    std::uniform_int_distribution<int> basica(1, 3);
    std::uniform_int_distribution<int> intermedia(1, 5);
    std::uniform_int_distribution<int> avanzada(1, 8);
    std::uniform_int_distribution<std::size_t> elegirObjeto(0, objetos.size() - 1);
    tocarInicio();

    for (std::size_t i = 0; i < planetas.size(); ++i) {
        mostrarPlaneta(planetas[i]);
        tocarNota(220 + static_cast<int>(i) * 15, 110);

        const Objeto& objeto = objetos[elegirObjeto(generador)];
        int a;
        int b;
        if (i < 3) {
            a = basica(generador);
            b = basica(generador);
        } else if (i < 6) {
            a = intermedia(generador);
            b = intermedia(generador);
        } else {
            a = avanzada(generador);
            b = avanzada(generador);
        }

        const int resultadoCorrecto = a + b;
        bool resuelta = false;
        for (int intento = 1; intento <= 5; ++intento) {
            std::cout << "\nCuenta los objetos:\n";
            mostrarObjetos(objeto, a);
            std::cout << "+ ";
            mostrarObjetos(objeto, b);
            std::cout << "\n\n" << a << ' '
                      << (a == 1 ? objeto.nombreSingular : objeto.nombre) << " + "
                      << b << ' '
                      << (b == 1 ? objeto.nombreSingular : objeto.nombre) << " = ";
            const int respuesta = leerRespuesta();
            if (respuesta == std::numeric_limits<int>::min()) {
                std::cout << "\nEntrada finalizada. Hasta pronto.\n";
                return false;
            }
            if (respuesta == resultadoCorrecto) {
                std::cout << "\033[92mCorrecto! AHORA SALTAS AL SIGUIENTE PLANETA.\033[0m\n\n";
                tocarCorrecto();
                mostrarPollitoYChiste(chistes[i]);
                resuelta = true;
                break;
            }

            if (intento < 5) {
                std::cout << "Incorrecto. 😢 Intentalo de nuevo. Te quedan "
                          << (5 - intento) << " intento(s).\n";
                tocarError();
            } else {
                std::cout << "Se acabaron los 5 intentos. El resultado era "
                          << resultadoCorrecto << ".\n"
                << "        😢\n"
                << "   .-\"\"-.\n"
                << "  / .--. \\\n"
                << " | (;_;) |\n"
                          << "  \\ '--' /\n"
                          << "   '-..-'\n"
                          << "No te rindas, puedes volver a intentarlo.\n";
                tocarDerrota();
            }
        }

        if (!resuelta) {
            std::cout << "\nFin del viaje espacial. Puedes intentarlo otra vez.\n";
            tocarDerrota();
            return false;
        }
    }

    std::cout << "\033[96m"
                 "                 /\\\n"
                 "                /  \\\n"
                 "               /NASA\\\n"
                 "              /______\\\n"
                 "             |  _  _  |\n"
                 "             | | || | |\n"
                 "             |_|_||_|_|\n"
                 "               / || \\\n"
                 "              /  ||  \\\n"
                 "             *   **   *\n"
                 "          NAVE ESPACIAL\n"
              << reiniciarColor;
    std::cout << "Llegaste al Sol! Lo hiciste increible! Ganaste el viaje espacial!\n";
    tocarVictoria();
    return true;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::random_device fuenteAleatoria;
    std::mt19937 generador(fuenteAleatoria());
    std::string respuesta;

    std::cout << "=== AVENTURA DE SUMAS POR EL SISTEMA SOLAR ===\n"
                 "Cuenta los objetos y resuelve las sumas para viajar desde Neptuno hasta el Sol.\n"
                 "Tienes 5 intentos por nivel. Las cantidades aumentan poco a poco.\n\n";

    do {
        const bool gano = jugar(generador);
        if (std::cin.eof()) {
            break;
        }

        std::cout << "\nQuieres repetir el juego? (si/no): ";
        std::cin >> respuesta;
        while (respuesta != "si" && respuesta != "SI" && respuesta != "Si" &&
               respuesta != "no" && respuesta != "NO" && respuesta != "No") {
            std::cout << "Escribe si o no: ";
            std::cin >> respuesta;
        }

        if (respuesta == "no" || respuesta == "NO" || respuesta == "No") {
            std::cout << "Opcion de salida seleccionada. Gracias por jugar!\n";
            break;
        }
        if (gano) {
            std::cout << "\nEmpezamos una nueva aventura desde cero con otras sumas!\n\n";
        } else {
            std::cout << "\nEmpezamos de nuevo desde cero con otras sumas!\n\n";
        }
    } while (true);

    return 0;
}
