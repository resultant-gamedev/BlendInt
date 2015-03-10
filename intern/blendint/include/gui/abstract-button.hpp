/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#pragma once

#include <string>

#include <Cpp/Events.hpp>

#include <core/margin.hpp>

#include <gui/abstract-icon.hpp>
#include <gui/text.hpp>
#include <gui/abstract-round-widget.hpp>

namespace BlendInt {

  class ButtonGroup;

  class AbstractButton: public AbstractRoundWidget
  {
  DISALLOW_COPY_AND_ASSIGN(AbstractButton);

  public:

    AbstractButton ();

    AbstractButton (const String& text);

    AbstractButton (const RefPtr<AbstractIcon>& icon);

    AbstractButton (const RefPtr<AbstractIcon>& icon, const String& text);

    virtual ~AbstractButton ();

    void SetDown (bool down);

    void SetCheckable (bool checkabel);

    void SetChecked (bool checked);

    virtual Size GetPreferredSize () const;

    const RefPtr<AbstractIcon>& icon () const
    {
      return icon_;
    }

    const RefPtr<Text>& text () const
    {
      return text_;
    }

    bool is_down () const
    {
      return button_flags_ & ButtonDownMask;
    }

    bool is_checked () const
    {
      return button_flags_ & ButtonCheckedMask;
    }

    bool is_checkable () const
    {
      return button_flags_ & ButtonCheckableMask;
    }

    bool is_pressed () const
    {
      return button_flags_ & ButtonPressedMask;
    }

    Cpp::EventRef<AbstractButton*> clicked ()
    {
      return clicked_;
    }

    Cpp::EventRef<AbstractButton*, bool> toggled ()
    {
      return toggled_;
    }

    Cpp::EventRef<AbstractButton*> pressed ()
    {
      return pressed_;
    }

  protected:

    virtual void PerformHoverIn (AbstractWindow* context);

    virtual void PerformHoverOut (AbstractWindow* context);

    virtual Response Draw (AbstractWindow* context) = 0;

    inline void set_icon (const RefPtr<AbstractIcon>& icon)
    {
      icon_ = icon;
    }

    inline void set_text (const String& text)
    {
      if (text_) {
        text_->SetText(text);
      } else {
        text_.reset(new Text(text));
      }
    }

    inline void set_font (const Font& font)
    {
      if (text_) text_->SetFont(font);
    }

    void DrawIconText ();

    inline void set_pressed (bool press)
    {
      if(press) {
        SETBIT(button_flags_, ButtonPressedMask);
      } else {
        CLRBIT(button_flags_, ButtonPressedMask);
      }
    }

    inline void set_down (bool down)
    {
      if (down) {
        SETBIT(button_flags_, ButtonDownMask);
      } else {
        CLRBIT(button_flags_, ButtonDownMask);
      }
    }

    inline void set_checkable (bool checkable)
    {
      if (checkable) {
        SETBIT(button_flags_, ButtonCheckableMask);
      } else {
        CLRBIT(button_flags_, ButtonCheckableMask);
      }
    }

    inline void set_checked (bool checked)
    {
      if(checked) {
        SETBIT(button_flags_, ButtonCheckedMask);
      } else {
        CLRBIT(button_flags_, ButtonCheckedMask);
      }
    }

    inline bool is_last_checked () const
    {
      return button_flags_ & ButtonLastCheckedMask;
    }

    inline void set_last_checked (bool checked)
    {
      if(checked) {
        SETBIT(button_flags_, ButtonLastCheckedMask);
      } else {
        CLRBIT(button_flags_, ButtonLastCheckedMask);
      }
    }

    inline void set_text (const RefPtr<Text>& text)
    {
      text_ = text;
    }

    static Margin kPadding;

    static int kIconTextSpace;  // the space between icon and text

  private:

    friend class ButtonGroup;

    enum ButtonFlagMask
    {
      ButtonPressedMask = 0x1 << 0,
      ButtonDownMask = 0x1 << 1,
      ButtonCheckableMask = 0x1 << 2,
      ButtonCheckedMask = 0x1 << 3,
      ButtonLastCheckedMask = 0x1 << 4,
    };

    virtual Response PerformMousePress (AbstractWindow* context);

    virtual Response PerformMouseRelease (AbstractWindow* context);

    virtual Response PerformMouseMove (AbstractWindow* context);

    RefPtr<AbstractIcon> icon_;

    RefPtr<Text> text_;

    int button_flags_;

    /**
     * @brief press event
     */
    Cpp::Event<AbstractButton*> pressed_;

    /**
     * @brief release event
     */
    Cpp::Event<AbstractButton*> released_;

    /**
     * @brief click event
     *
     * Mouse press and release in the button causes a clicked event.
     */
    Cpp::Event<AbstractButton*> clicked_;

    Cpp::Event<AbstractButton*, bool> toggled_;

    ButtonGroup* group_;
  };

} /* namespace BIL */
