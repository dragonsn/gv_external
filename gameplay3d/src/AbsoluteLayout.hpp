#include "Base.h"
#include "Control.h"
#include "AbsoluteLayout.h"
#include "Container.h"

namespace gameplay
{

static AbsoluteLayout* __layout_instance;

AbsoluteLayout::AbsoluteLayout()
{
}

AbsoluteLayout::~AbsoluteLayout()
{
    __layout_instance = NULL;
}

AbsoluteLayout* AbsoluteLayout::create()
{
    if (!__layout_instance)
    {
        __layout_instance = new AbsoluteLayout();
    }
    else
    {
        __layout_instance->addRef();
    }

    return __layout_instance;
}

Layout::Type AbsoluteLayout::getType()
{
    return Layout::LAYOUT_ABSOLUTE;
}

void AbsoluteLayout::update(const Container* container, const Vector2& offset)
{
    GP_ASSERT(container);

    // An AbsoluteLayout does nothing to modify the layout of Controls.
    const std::vector<Control*>& controls = container->getControls();
    for (size_t i = 0, count = controls.size(); i < count; i++)
    {
        Control* control = controls[i];
        GP_ASSERT(control);

        align(control, container);
        control->update(container, offset);
    }
}

}