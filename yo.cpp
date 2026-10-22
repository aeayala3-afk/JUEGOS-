#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

constexpr const char* reiniciarColor = "\033[0m";
constexpr std::array<const char*, 6> colores{{
    "\033[95m", "\033[96m", "\033[93m",
    "\033[92m", "\033[94m", "\033[91m"
}};

struct Pregunta {
    const char* texto;
    const char* escena;
    std::array<const char*, 3> opciones;
    std::size_t respuestaCorrecta;
    const char* comentario;
};

constexpr std::array<Pregunta, 14> preguntas{{
    {"¿Con qué parte ves la tele?", "📺🐱🍿",
     {{"👀 OJOS", "👂 OREJAS", "👃 NARIZ"}}, 0,
     "¡Eso! Tus ojos ven hasta cuando el gato tapa la pantalla."},
    {"¿Con qué parte silbas para llamar a tu perro?", "🐶🎵",
     {{"👄 LABIOS", "🦵 RODILLAS", "🖐️ MANOS"}}, 0,
    "¡Muy bien! Las rodillas todavía no aprendieron a silbar."},
    {"¿Con qué parte juegas videojuegos?", "🎮👾🕹️",
     {{"🖐️ MANOS", "👂 OREJAS", "🦶 PIES"}}, 0,
     "¡Correcto! Tus manos son campeonas de los botones."},
    {"¿Con qué parte escuchas tu canción favorita?", "🎧🎶💃",
     {{"👂 OREJAS", "👀 OJOS", "🦷 DIENTES"}}, 0,
    "¡Exacto! Las orejas tienen palco VIP para la música."},
    {"¿Con qué parte hueles una galleta recién hecha?", "🍪🤤",
     {{"👃 NARIZ", "👅 LENGUA", "🦵 RODILLAS"}}, 0,
    "¡Bien! La nariz detectó una galleta a kilómetros."},
    {"¿Con qué parte pateas una pelota?", "⚽🥅",
     {{"🦶 PIES", "🖐️ MANOS", "👃 NARIZ"}}, 0,
     "¡Gol! Tus pies hicieron el pase perfecto."},
    {"¿Con qué parte das un aplauso de campeón?", "👏🏆✨",
     {{"🖐️ MANOS", "👂 OREJAS", "🦷 DIENTES"}}, 0,
     "¡Bravo! Tus manos piden otra ronda de aplausos."},
    {"¿Qué parte te ayuda a pensar y recordar dónde escondiste los dulces?", "🧠🍬🤔",
     {{"🧠 CEREBRO", "🦶 TALONES", "💪 CODOS"}}, 0,
    "¡Muy bien! Tu cerebro encontró el recuerdo... ¿y los dulces?"},
    {"¿Con qué parte comes un plato enorme de espagueti?", "🍝😋",
     {{"👄 BOCA", "👂 OREJAS", "🖐️ CODO"}}, 0,
    "¡Sí! La boca es la puerta oficial de los espaguetis."},
    {"¿Con qué parte abrazas a tu oso de peluche?", "🧸🤗",
     {{"💪 BRAZOS", "🦵 TOBILLOS", "👃 NARIZ"}}, 0,
    "¡Awww! Tus brazos dan abrazos de primera categoría."},
    {"¿Qué parte doblas para hacer el baile del robot?", "🤖🪩",
     {{"🦵 RODILLAS", "👅 LENGUA", "👂 OREJAS"}}, 0,
    "¡Robot activado! Tus rodillas hacen un gran movimiento mecánico."},
    {"¿Qué parte muestras cuando sonríes para una foto?", "📸😁",
     {{"🦷 DIENTES", "🖐️ PALMAS", "🦶 DEDOS DEL PIE"}}, 0,
    "¡Qué sonrisa! Hasta la cámara se puso contenta."},
    {"¿Con qué parte pruebas si el helado está delicioso?", "🍦😋",
     {{"👅 LENGUA", "👃 NARIZ", "💪 CODO"}}, 0,
     "¡Delicioso! La lengua es experta catadora de helado."},
    {"¿Con qué parte te tapas los ojos cuando juegas a las escondidas?", "🙈🌳",
     {{"🖐️ MANOS", "🦶 PIES", "🦷 DIENTES"}}, 0,
     "¡Muy bien! Ahora cuenta hasta diez... ¡sin espiar!"}
}};

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

bool jugar(std::mt19937& generador) {
    std::array<std::size_t, preguntas.size()> orden{};
    for (std::size_t i = 0; i < orden.size(); ++i) {
        orden[i] = i;
    }
    std::shuffle(orden.begin(), orden.end(), generador);

    int puntos = 0;
    std::cout << colores[5]
              << "\n🌈✨🧠 ¡YO PUEDO! 🧠✨🌈\n"
                 "      .-=========-.\n"
                 "     /  👀  👂  👃  \\\n"
                 "    |   👄  🖐️  🦶   |\n"
                 "     \\  💪  🧠  /\n"
                 "      '-=======-'\n"
              << reiniciarColor
              << "¡Hola, explorador del cuerpo! 🚀\n"
                 "Responde preguntas, elige entre 3 opciones y gana puntos.\n"
                 "Tienes hasta 3 intentos por pregunta. ¡A jugar! 🎉\n";

    for (std::size_t ronda = 0; ronda < orden.size(); ++ronda) {
        const Pregunta& pregunta = preguntas[orden[ronda]];
        std::array<std::size_t, 3> ordenOpciones{{0, 1, 2}};
        std::shuffle(ordenOpciones.begin(), ordenOpciones.end(), generador);

        std::size_t posicionCorrecta = 0;
        for (std::size_t i = 0; i < ordenOpciones.size(); ++i) {
            if (ordenOpciones[i] == pregunta.respuestaCorrecta) {
                posicionCorrecta = i;
                break;
            }
        }

        const char* color = colores[ronda % colores.size()];
        std::cout << '\n' << color << "🎈 PREGUNTA " << (ronda + 1)
                  << " DE " << orden.size() << " 🎈\n"
                  << pregunta.escena << "  " << pregunta.texto << '\n'
                  << reiniciarColor;

        for (std::size_t i = 0; i < ordenOpciones.size(); ++i) {
            std::cout << colores[(i + ronda) % colores.size()]
                      << "  " << (i + 1) << ") "
                      << pregunta.opciones[ordenOpciones[i]]
                      << reiniciarColor << '\n';
        }

        bool acerto = false;
        for (int intento = 1; intento <= 3; ++intento) {
            std::cout << "Tu respuesta: ";
            const int respuesta = leerRespuesta();
            if (respuesta == std::numeric_limits<int>::min()) {
                std::cout << "\n¡Gracias por jugar! ¡Hasta pronto! 👋\n";
                return false;
            }

            if (static_cast<std::size_t>(respuesta - 1) == posicionCorrecta) {
                acerto = true;
                ++puntos;
                std::cout << colores[3] << "¡Correcto! " << pregunta.comentario
                          << " 🌟\n" << reiniciarColor;
                break;
            }

            if (intento < 3) {
                std::cout << colores[0]
                          << "¡Casi! Piensa un poquito más. ¡Tú puedes! 💡\n"
                          << reiniciarColor;
            }
        }

        if (!acerto) {
            std::cout << colores[2] << "La respuesta era: "
                      << pregunta.opciones[pregunta.respuestaCorrecta]
                      << ". ¡Ya aprendiste algo nuevo! 🌱\n" << reiniciarColor;
        }
    }

    std::cout << colores[5] << "\n🎊 ¡Terminaste la aventura! 🎊\n"
              << "Puntaje: " << puntos << " de " << preguntas.size() << ".\n";
    if (puntos == static_cast<int>(preguntas.size())) {
        std::cout << "¡Puntaje perfecto! Eres una estrella del cuerpo humano. 🌟\n";
    } else if (puntos >= 10) {
        std::cout << "¡Gran trabajo! Sabes un monton sobre tu cuerpo. 🏆\n";
    } else {
        std::cout << "¡Buen intento! Cada pregunta te hizo más sabio. 🧠\n";
    }
    std::cout << reiniciarColor;
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
            if (std::cin.eof()) {
                break;
            }
        }
    } while (respuesta == "si" || respuesta == "SI" || respuesta == "Si");

    std::cout << "¡Gracias por jugar a YO PUEDO! 🧠🌈\n";
    return 0;
}