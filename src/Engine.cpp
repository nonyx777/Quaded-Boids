#include "../include/Engine.hpp"

Engine::Engine() : Window()
{
    this->initWindow();
    this->initImGui();
}

void Engine::initWindow()
{
    this->video_mode.width = GLOBAL::window_width;
    this->video_mode.height = GLOBAL::window_height;
    this->window = new sf::RenderWindow(this->video_mode, "Proto", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
    this->window->setFramerateLimit(60);
}

void Engine::pollEvent()
{
    while (this->window->pollEvent(this->event))
    {
        this->eventImGui();
        switch (this->event.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->event.key.code == sf::Keyboard::Q)
                scene->generateVehicles(this->mouse_position_view);
            break;
        }
    }
}

void Engine::update(float dt)
{
    this->pollEvent();
    this->mouse_position = sf::Mouse::getPosition(*this->window);
    this->mouse_position_view = this->window->mapPixelToCoords(this->mouse_position);

    scene->update(this->mouse_position_view, dt);
    this->updateImGui();
    ImGui::Begin("Proto");
    ImGui::Text("FPS: %f", GLOBAL::fps);
    ImGui::End();
}
void Engine::render(sf::RenderTarget *target)
{
    //....
}
void Engine::render()
{
    this->window->clear(sf::Color::Black);
    scene->render(this->window);
    this->renderImGui();
    this->window->display();
}