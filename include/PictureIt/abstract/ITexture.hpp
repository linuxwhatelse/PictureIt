#pragma once

#include <memory>


class ITexture {
    protected:
        /*!
         * @brief The textures width in pixel
         */
        int width;

        /*!
         * @brief The textures height in pixel
         */
        int height;

    public:
        enum MODE {
            STRETCH,
            CENTER,
            SCALE,
            ZOOM
        };

        std::unique_ptr<int> p;
        ITexture() {
            p = std::make_unique<int>(1);
        };
        virtual ~ITexture() = default;

        /*!
         * The windows width this texture will be rendered in
         */
        int win_width = -1;

        /*!
         * The windows height this texture will be rendered in
         */
        int win_height = -1;

        /*!
         * @brief Position of the textures top left corner
         * where index 0 = x-axis and index 1 = y-axis
         */
        float top_left[2];

        /*!
         * @brief Position of the textures top right corner
         * where index 0 = x-axis and index 1 = y-axis
         */
        float top_right[2];

        /*!
         * @brief Position of the textures bottom left corner
         * where index 0 = x-axis and index 1 = y-axis
         */
        float bottom_left[2];

        /*!
         * @brief Position of the textures bottom right corner
         * where index 0 = x-axis and index 1 = y-axis
         */
        float bottom_right[2];

        /*!
         * @brief Position of the vertex top left corner
         * where index 0 = x-axis and index 1 = y-axis
         */
        float vert_top_left[2];

        /*!
         * @brief Position of the vertex top right corner
         * where index 0 = x-axis and index 1 = y-axis
         */
        float vert_top_right[2];

        /*!
         * @brief Position of the vertex bottom left corner
         * where index 0 = x-axis and index 1 = y-axis
         */
        float vert_bottom_left[2];

        /*!
         * @brief Position of the vertex bottom right corner
         * where index 0 = x-axis and index 1 = y-axis
         */
        float vert_bottom_right[2];

        /*!
         * @brief The textures red value
         */
        float red = 1.0;

        /*!
         * @brief The textures green value
         */
        float green = 1.0;

        /*!
         * @brief The textures blue value
         */
        float blue = 1.0;

        /*!
         * @brief The textures alpha value
         */
        float alpha = 1.0;

        void stretch();
        void center();
        void scale();
        void zoom();

        /*!
         * Whether or not this is a valid, drawable texture
         */
        virtual bool is_texture() = 0 ;

        /*!
         * @brief Load an image into this texture object
         * @param img_path Absolute path to a image which to create a Texture
         *        object for
         * @return true on success, false otherwise
         */
        virtual bool load_image(const char *img_path) = 0;

        /*!
         * @brief Render this texture according to its members values
         */
        virtual void render() = 0;
};
