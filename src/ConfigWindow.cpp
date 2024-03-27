#include "../include/ConfigWindow.hpp"

ConfigWindow::ConfigWindow() : Window()
{
    this->initWindow();
    this->initImGui();
}

void ConfigWindow::initWindow()
{
    this->video_mode.width = GLOBAL::config_window_width;
    this->video_mode.height = GLOBAL::config_window_height;
    this->window = new sf::RenderWindow(this->video_mode, "Configuration Window", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}

void ConfigWindow::update(float dt)
{
    this->pollEvent();

    this->updateImGui();
    ImGui::Begin("Parameters");
    ImGui::SetWindowSize(ImVec2(this->window->getSize()));

    // configure engine window size
    ImGui::Text("Window Size");
    ImGui::InputInt("Window Height", &GLOBAL::window_height);
    ImGui::InputInt("Window Width", &GLOBAL::window_width);

    GLOBAL::window_height = Math::_clampOnRange(GLOBAL::window_height, 400, 1000);
    GLOBAL::window_width = Math::_clampOnRange(GLOBAL::window_width, 400, 1000);

    //Simulaiton parameters
    ImGui::Text("Simulation parameters");
    ImGui::InputInt("boid size", &GLOBAL::size);
    ImGui::InputInt("range offset", &GLOBAL::range_offset);

    ImGui::End();
}

void ConfigWindow::render(sf::RenderTarget *target)
{
    //....
}