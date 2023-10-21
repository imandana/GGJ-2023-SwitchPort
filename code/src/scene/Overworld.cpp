#include "scene/Overworld.h"

Overworld::Overworld() :
	Scene(),
	main_character("../resources/Actor_sangoku01.png"),
	camera(sf::FloatRect(0, 0, 800, 600))
{
	tiled2Sfml.setCollisionLayer({ 1, 4 });
	tiled2Sfml.tileParser("../resources/maps/", "prologue.json");
	main_character.setTilemap(tiled2Sfml);

	auto& render = ServiceLocator::getService<RenderManager>();
	render.setNeedRedraw(true);

}

void Overworld::update(float dt)
{
	camera.setCenter(main_character.sprite.getPosition());
	main_character.update(dt);
}

void Overworld::static_draw(sf::RenderTexture& render_tex)
{
	render_tex.setView(camera);
	for (size_t layer_idx = 0; layer_idx < tiled2Sfml.getTilemapData().layers.size(); layer_idx++)
	{
		if (layer_idx == player_layer)
		{
			main_character.draw(render_tex);
		}
		else
		{
			for (const auto& tile : tiled2Sfml.getTileSprite())
			{
				if (tile.layer_index == layer_idx &&
					isNearPlayer(tile.sprite.getPosition(), main_character.sprite.getPosition(), render_distance))
				{
					render_tex.draw(tile.sprite);
				}
			}
		}
	}
}

void Overworld::dynamic_draw(sf::RenderWindow& window)
{

}

bool Overworld::isNearPlayer(const sf::Vector2f& tile_position, const sf::Vector2f& player_position, float render_distance)
{
	float dx = tile_position.x - player_position.x;
	float dy = tile_position.y - player_position.y;
	float distance_squared = dx * dx + dy * dy;
	return distance_squared <= render_distance * render_distance;
}
