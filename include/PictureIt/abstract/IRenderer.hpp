#pragma once

#include <memory>

class IRenderer {
    public:
        int window_width = 0;
        int window_height = 0;

        std::unique_ptr<int> p;
        IRenderer() {
            p = std::make_unique<int>(1);
        };
        virtual ~IRenderer() = default;

        /*!
         * @brief Used to initialize the render-engine. This will be run only
         *        once upon initializing PictureIt
         */
        virtual void init() = 0;

        /*!
         * @brief Used to prepare the frame that needs drawing
         */
        virtual void prepare() = 0;

        /*!
         * @brief Used to finish the current frame
         */
        virtual void finish() = 0;
};
