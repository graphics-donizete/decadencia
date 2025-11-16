#include <iostream>

#include <SFML/Graphics.hpp>

#include "skia/include/gpu/ganesh/GrBackendSurface.h"
#include "skia/include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "skia/include/gpu/ganesh/gl/GrGLBackendSurface.h"

#include "skia/include/core/SkPaint.h"
#include "skia/include/core/SkCanvas.h"
#include "skia/include/core/SkColorSpace.h"

#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"

#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"

void draw_line(SkCanvas *canvas)
{
    SkPaint p;

    p.setColor(SK_ColorRED);
    p.setAntiAlias(true);
    p.setStyle(SkPaint::kStroke_Style);
    p.setStrokeWidth(10);

    canvas->drawLine(20, 20, 100, 100, p);
}

void draw_square(SkCanvas *canvas)
{
    SkPaint paint;
    paint.setColor(SK_ColorBLUE);
    canvas->drawRect({0, 0, 250, 250}, paint);
}

int main(int, char **)
{
    auto window = sf::RenderWindow(sf::VideoMode({500, 500}), "CMake SFML Project");
    window.setFramerateLimit(60);

    // Setup Skia OpenGL context
    sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
    sk_sp<GrDirectContext> context = GrDirectContexts::MakeGL(interface);

    // Define the image properties and create a Skia GPU surface
    SkImageInfo imageinfo = SkImageInfo::MakeN32Premul(500, 500);
    sk_sp<SkSurface> surface = SkSurfaces::RenderTarget(context.get(), skgpu::Budgeted::kNo, imageinfo);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        (void)window.setActive(true);

        draw_square(surface->getCanvas());
        draw_line(surface->getCanvas());

        context->flushAndSubmit();

        window.display();
    }
}