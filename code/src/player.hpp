#pragma once
#include "object.hpp"
#include "system/ecs/entity.hpp"

class player : public entity
{
public:
	player(const std::string& image_path) 
	{
		if (!player_texture.loadFromFile(image_path))
		{
			printf_s("Failed to load file!");
		}

		sprite = add_component<sprite_component>(player_texture);
		transform = add_component<transform_component>(*this);

		sprite->set_texture_rect(0, 0, sprite_width, sprite_height);
		transform->set_position(0.0f, 0.0f);

		std::vector<std::vector<sf::IntRect>> all_frames = {
			{ // Down
				{48, 0, sprite_width, sprite_height},
				{0, 0, sprite_width, sprite_height},
				{48, 0, sprite_width, sprite_height},
				{96, 0, sprite_width, sprite_height},
			},
			{ // Left
				{48, 48, sprite_width, sprite_height},
				{0, 48, sprite_width, sprite_height},
				{48, 48, sprite_width, sprite_height},
				{96, 48, sprite_width, sprite_height},
			},
			{ // Right
				{48, 96, sprite_width, sprite_height},
				{0, 96, sprite_width, sprite_height},
				{48, 96, sprite_width, sprite_height},
				{96, 96, sprite_width, sprite_height},
			},
			{ // Up
				{48, 144, sprite_width, sprite_height},
				{0, 144, sprite_width, sprite_height},
				{48, 144, sprite_width, sprite_height},
				{96, 144, sprite_width, sprite_height},
			}
		};

		// Create the animated sprite objects
		player_animation = std::make_unique<animation_component>(sprite, all_frames, 0.1f);
	}

	~player() = default;

	void movement(float dt)
	{
		if (is_moving)
		{
			elapsed_time += dt;
			float move_fraction = elapsed_time / move_duration;

			if (move_fraction >= 1.0f)
			{
				is_moving = false;
				move_fraction = 1.0f;
			}

			transform->set_position(initial_position + sf::Vector2f(move_fraction * move_direction.x, move_fraction * move_direction.y));


			int row = -1;
			if (move_direction.y > 0) row = 0;
			else if (move_direction.x < 0) row = 1;
			else if (move_direction.x > 0) row = 2;
			else if (move_direction.y < 0) row = 3;

			if (row != -1)
			{
				player_animation->update(dt, row);
			}
		}
		else
		{
			sf::Vector2f dest;

			int row = -1;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				dest = sf::Vector2f(-grid_size, 0.0f);
				row = 1;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				dest = sf::Vector2f(0.0f, -grid_size);
				row = 3;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				dest = sf::Vector2f(0.0f, grid_size);
				row = 0;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				dest = sf::Vector2f(grid_size, 0.0f);
				row = 2;
			}

			if (row != -1)
			{
				is_moving = true;
				move_direction = dest;
				initial_position = transform->get_position();
				elapsed_time = 0;
			}
		}
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(sprite->get_sprite());
	}

	std::shared_ptr<sprite_component> sprite;
	std::shared_ptr<transform_component> transform;
	std::unique_ptr<animation_component> player_animation;

private:
	sf::Texture player_texture;
	int sprite_width = 48, sprite_height = 48;
	float speed = 1000.0f;

	bool is_moving = false;
	sf::Vector2f initial_position;
	sf::Vector2f move_direction;
	float elapsed_time;
	float grid_size = 32.0f;
	float move_duration = 0.2f;
};