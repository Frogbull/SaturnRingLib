#pragma once

#include "srl_base.hpp"

/** @brief Television control
 */
namespace SRL
{
    namespace Types
    {
        /** @brief Area resolution
         */
        struct Resolution final
        {
            /** @brief Construct a new Resolution tuple
             */
            Resolution() : Width(0), Height(0) {}

            /** @brief Construct a new Resolution tuple
             * @param width Area width
             * @param height Area height
             */
            Resolution(const int16_t width, const int16_t height) : Width(width), Height(height) {}

            /** @brief Area width
             */
            int16_t Width;

            /** @brief Area height
             */
            int16_t Height;
        };
    }

    /** @brief Contains TV resolutions
     */
    class TV final
    {
    public:
    
        /** @brief Available TV resolutions
         */
        enum class Resolutions
        {
            Normal320x224 = TV_320x224,
            Normal320x240 = TV_320x240,
            Normal320x256 = TV_320x256,
            Normal352x224 = TV_352x224,
            Normal352x240 = TV_352x240,
            Normal352x256 = TV_352x256,

            Interlaced640x224 = TV_640x224,
            Interlaced640x240 = TV_640x240,
            Interlaced704x224 = TV_704x224,
            Interlaced704x240 = TV_704x240,

            Normal320x448i = 16,
            Normal320x480i = 17,
            Normal352x448 = 20,
            Normal352x480 = 21,

            Interlaced640x448i = 24,
            Interlaced640x480i = 25,
            Interlaced704x448 = 28,
            Interlaced704x480 = 29
        };

    private:

        /** @brief Befriend core
         */
        friend SRL::Core;

        /** @brief Size of the screen
         */
        inline static SRL::Types::Resolution ScreenSize;

        /** @brief Contains the current resolution setting
         */
        inline static Resolutions CurrentResolution;

        /** @brief Make class purely static
         */
        TV() = delete;

        /** @brief Make class purely static
         */
        ~TV() = delete;

        /** @brief Set the screen resolution state
         * @param resolution Current resolution
         */
        static void SetScreenSize(const Resolutions resolution)
        {
            TV::CurrentResolution = resolution;

            switch (resolution)
            {
            // Normal resolutions
            case Resolutions::Normal320x224:
                TV::ScreenSize = SRL::Types::Resolution(320, 224);
                break;
            
            case Resolutions::Normal320x240:
                TV::ScreenSize = SRL::Types::Resolution(320, 240);
                break;

            case Resolutions::Normal320x256:
                TV::ScreenSize = SRL::Types::Resolution(320, 256);
                break;

            case Resolutions::Normal352x224:
                TV::ScreenSize = SRL::Types::Resolution(352, 224);
                break;

            case Resolutions::Normal352x240:
                TV::ScreenSize = SRL::Types::Resolution(352, 240);
                break;

            case Resolutions::Normal352x256:
                TV::ScreenSize = SRL::Types::Resolution(352, 256);
                break;

            case Resolutions::Normal352x448:
                TV::ScreenSize = SRL::Types::Resolution(352, 448);
                break;
            case Resolutions::Normal352x480:
                TV::ScreenSize = SRL::Types::Resolution(352, 480);
                break;

            case Resolutions::Normal320x448i:
                TV::ScreenSize = SRL::Types::Resolution(320, 448);
                break;
                
            case Resolutions::Normal320x480i:
                TV::ScreenSize = SRL::Types::Resolution(320, 480);
                break;

            // Interlaced resolutions
            case Resolutions::Interlaced640x224:
                TV::ScreenSize = SRL::Types::Resolution(640, 224);
                break;

            case Resolutions::Interlaced640x240:
                TV::ScreenSize = SRL::Types::Resolution(640, 240);
                break;

            case Resolutions::Interlaced704x224:
                TV::ScreenSize = SRL::Types::Resolution(704, 224);
                break;

            case Resolutions::Interlaced704x240:
                TV::ScreenSize = SRL::Types::Resolution(704, 240);
                break;

            case Resolutions::Interlaced640x448i:
                TV::ScreenSize = SRL::Types::Resolution(640, 448);
                break;

            case Resolutions::Interlaced640x480i:
                TV::ScreenSize = SRL::Types::Resolution(640, 480);
                break;

            case Resolutions::Interlaced704x448:
                TV::ScreenSize = SRL::Types::Resolution(704, 448);
                break;

            case Resolutions::Interlaced704x480:
                TV::ScreenSize = SRL::Types::Resolution(704, 480);
                break;

            default:
                break;
            }

            TV::Width = TV::ScreenSize.Width;
            TV::Height = TV::ScreenSize.Height;
        }

    public:

        /** @brief Turn on TV display
         */
        static void TVOn()
        {
            slTVOn();
        }

        /** @brief Turn off TV display
         */
        static void TVOff()
        {
            slTVOff();
        }

        /** @brief Read only screen width
         */
        inline static int16_t Width;

        /** @brief Read only screen height
         */
        inline static int16_t Height;

        /** @brief Read only screen resolution mode
         */
        inline static TV::Resolutions Resolution;
    };
};
