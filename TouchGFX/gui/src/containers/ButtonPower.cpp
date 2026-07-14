#include <gui/containers/ButtonPower.hpp>
#include <images/BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

ButtonPower::ButtonPower()
{

}

void ButtonPower::initialize()
{
    ButtonPowerBase::initialize();

    // Set up border box (slightly larger to act as a border/highlight)
    borderBox.setPosition(0, 0, 70, 70);
    borderBox.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255)); // White initially to blend with BG

    // Insert borderBox behind the power button
    remove(power);
    add(borderBox);
    add(power);
}

void ButtonPower::setUploadState(bool isUploading, bool isLearning)
{
    if (isUploading)
    {
        if (isLearning)
        {
            // Learning state: Green border
            borderBox.setColor(touchgfx::Color::getColorFromRGB(50, 180, 50));
            power.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_IMAGE_CIRCLE_70_70_FF0000_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_IMAGE_CIRCLE_70_70_000000_SVG_ID));
        }
        else
        {
            // Waiting state: Yellow border + Black center
            borderBox.setColor(touchgfx::Color::getColorFromRGB(230, 200, 0));
            power.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_IMAGE_CIRCLE_70_70_000000_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_IMAGE_CIRCLE_70_70_000000_SVG_ID));
        }
    }
    else
    {
        // Normal state: White border (invisible) + Red center
        borderBox.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
        power.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_IMAGE_CIRCLE_70_70_FF0000_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_IMAGE_CIRCLE_70_70_000000_SVG_ID));
    }
    borderBox.invalidate();
    power.invalidate();
}
