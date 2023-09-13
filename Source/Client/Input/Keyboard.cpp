#include <Input/Keyboard.hpp>

#include <defines.h>

#include <toml.hpp>

#include <fstream>
#include <iostream>

namespace Client
{
    namespace Input
    {
        void KeyboardProfile::ReadKeyboardProfile(const std::string &filename)
        {
            const auto data = toml::parse(filename);
            const auto keys = toml::find(data, "keys");

            if (keys.contains("left"))
                left = SDL_GetScancodeFromName(toml::find<std::string>(keys, "left").c_str());
            if (keys.contains("right"))
                right = SDL_GetScancodeFromName(toml::find<std::string>(keys, "right").c_str());
            if (keys.contains("instantLeft"))
                instantLeft = SDL_GetScancodeFromName(toml::find<std::string>(keys, "instantLeft").c_str());
            if (keys.contains("instantRight"))
                instantRight = SDL_GetScancodeFromName(toml::find<std::string>(keys, "instantRight").c_str());
            if (keys.contains("rotateLeft"))
                rotateLeft = SDL_GetScancodeFromName(toml::find<std::string>(keys, "rotateLeft").c_str());
            if (keys.contains("rotateRight"))
                rotateRight = SDL_GetScancodeFromName(toml::find<std::string>(keys, "rotateRight").c_str());
            if (keys.contains("flip"))
                flip = SDL_GetScancodeFromName(toml::find<std::string>(keys, "flip").c_str());
            if (keys.contains("softDrop"))  
                softDrop = SDL_GetScancodeFromName(toml::find<std::string>(keys, "softDrop").c_str());
            if (keys.contains("sonicDrop")) 
                sonicDrop = SDL_GetScancodeFromName(toml::find<std::string>(keys, "sonicDrop").c_str());
            if (keys.contains("hardDrop"))
                hardDrop = SDL_GetScancodeFromName(toml::find<std::string>(keys, "hardDrop").c_str());
            
            const auto repeat = toml::find(data, "repeat");
            if (repeat.contains("period"))
                repeatPeriod = toml::find<int>(repeat, "period");
            if (repeat.contains("delay"))
                repeatDelay = toml::find<int>(repeat, "delay");
            if (repeat.contains("softdropPeriod"))
                softdropPeriod = toml::find<int>(repeat, "softdropPeriod");
            if (repeat.contains("softdropDelay"))
                softdropDelay = toml::find<int>(repeat, "softdropDelay");
        }

        void KeyboardProfile::WriteKeyboardProfile(const std::string &filename)
        {
            std::ofstream file(filename, std::ios::out);
            toml::value data;

            // Add keys 
            toml::table keys;
            keys["left"] = SDL_GetScancodeName(left);
            keys["right"] = SDL_GetScancodeName(right);
            keys["instantLeft"] = SDL_GetScancodeName(instantLeft);
            keys["instantRight"] = SDL_GetScancodeName(instantRight);
            keys["rotateLeft"] = SDL_GetScancodeName(rotateLeft);
            keys["rotateRight"] = SDL_GetScancodeName(rotateRight);
            keys["flip"] = SDL_GetScancodeName(flip);
            keys["softDrop"] = SDL_GetScancodeName(softDrop);
            keys["sonicDrop"] = SDL_GetScancodeName(sonicDrop);
            keys["hardDrop"] = SDL_GetScancodeName(hardDrop);
            
            // Add repeat
            toml::table repeat;
            repeat["period"] = repeatPeriod;
            repeat["delay"] = repeatDelay;
            repeat["softdropPeriod"] = softdropPeriod;
            repeat["softdropDelay"] = softdropDelay;

            data["keys"] = keys;
            data["repeat"] = repeat;

            file << data << std::endl;
            file.close();
        }


        void Keyboard::Update(int elapsedMilliseconds)
        {
            const u8 *keyState = SDL_GetKeyboardState(NULL);
            if (lastKey == profile.left && keyState[profile.left])
            {
                timeUntilRepeat -= elapsedMilliseconds;
                if (timeUntilRepeat <= 0)
                {
                    if (profile.repeatPeriod == 0)
                    {
                        output.push(Core::Game::GameEvent::INSTANT_LEFT);
                        return;
                    }

                    output.push(Core::Game::GameEvent::MOVE_LEFT);
                    timeUntilRepeat = profile.repeatPeriod;
                }
            }
            else if (lastKey == profile.right && keyState[profile.right])
            {
                timeUntilRepeat -= elapsedMilliseconds;
                if (timeUntilRepeat <= 0)
                {
                    if (profile.repeatPeriod == 0)
                    {
                        output.push(Core::Game::GameEvent::INSTANT_RIGHT);
                        return;
                    }

                    output.push(Core::Game::GameEvent::MOVE_RIGHT);
                    timeUntilRepeat = profile.repeatPeriod;
                }
            }
            if (keyState[profile.softDrop])
            {
                timeUntilSoftdrop -= elapsedMilliseconds;
                if (timeUntilSoftdrop <= 0)
                {
                    // Just for completeness
                    if (profile.softdropPeriod == 0)
                    {
                        output.push(Core::Game::GameEvent::HARD_DROP);
                        return;
                    }

                    output.push(Core::Game::GameEvent::SOFT_DROP);
                    timeUntilSoftdrop = profile.softdropPeriod;
                }
            }
        }

        void Keyboard::KeyDown(const SDL_KeyboardEvent &event)
        {
            SDL_Scancode scancode = event.keysym.scancode;

            if (scancode == profile.left)
            {
                output.push(Core::Game::GameEvent::MOVE_LEFT);
                lastKey = scancode;
                timeUntilRepeat = profile.repeatDelay;
                return;
            }

            if (scancode == profile.right)
            {
                output.push(Core::Game::GameEvent::MOVE_RIGHT);
                lastKey = scancode;
                timeUntilRepeat = profile.repeatDelay;
                return;
            }

            if (scancode == profile.rotateLeft)
            {
                output.push(Core::Game::GameEvent::RCCW);
                return;
            }

            if (scancode == profile.rotateRight)
            {
                output.push(Core::Game::GameEvent::RCW);
                return;
            }

            if (scancode == profile.flip)
            {
                output.push(Core::Game::GameEvent::FLIP);
                return;
            }

            if (scancode == profile.softDrop)
            {
                output.push(Core::Game::GameEvent::SOFT_DROP);
                timeUntilSoftdrop = profile.softdropDelay;
                return;
            }

            if (scancode == profile.sonicDrop)
            {
                output.push(Core::Game::GameEvent::SONIC_DROP);
                return;
            }

            if (scancode == profile.hardDrop)
            {
                output.push(Core::Game::GameEvent::HARD_DROP);
                return;
            }

            if (scancode == profile.instantLeft)
            {
                output.push(Core::Game::GameEvent::INSTANT_LEFT);
                return;
            }

            if (scancode == profile.instantRight)
            {
                output.push(Core::Game::GameEvent::INSTANT_RIGHT);
                return;
            }
        }

        Core::Game::EventStream Keyboard::GetInputStream()
        {
            return output.getStream();
        }
    }
}