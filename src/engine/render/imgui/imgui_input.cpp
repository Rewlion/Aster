#include "imgui.h"

#include <engine/input/drivers/buttons.h>

namespace Engine::imgui
{
  auto Manager::onMouseButtonStateChanged(const Input::IPointer& device,
                                               const int key, 
                                               const bool pressed) -> Input::InputRouterProcessStatus
  {
    ImGuiIO& io = ImGui::GetIO();
    int btn = -1;
    switch (key)
    {
      case Input::BKEY_LBMOUSE: btn = 0; break;
      case Input::BKEY_RBMOUSE: btn = 1; break;
      case Input::BKEY_MBMOUSE: btn = 2; break;
    }
    if (btn != -1)
      io.AddMouseButtonEvent(btn, pressed);

    return Input::InputRouterProcessStatus::ProcessFurther;
  }

  auto Manager::onMouseMove(const Input::IPointer& device,
                                 const int2 new_pos,
                                 const int2 delta) -> Input::InputRouterProcessStatus
  {
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)new_pos.x, (float)new_pos.y);
    return Input::InputRouterProcessStatus::ProcessFurther;
  }

  auto Manager::onKeyStateChanged(const Input::IKeyboard& device,
                                       const int key,
                                       const bool pressed) -> Input::InputRouterProcessStatus
  {
    ImGuiIO& io = ImGui::GetIO();

    const bool shiftPressed = device.isButtonPressed(Input::BKEY_LSHIFT) || 
                              device.isButtonPressed(Input::BKEY_RSHIFT);

    int btn = -1;
    int ch = -1;

    const auto toCh = [shiftPressed](char lc, char hc) { return shiftPressed ? hc : lc; };

    switch (key)
    {        
      case Input::BKEY_BACKSPACE:         btn = ImGuiKey_Backspace;                            break;
      case Input::BKEY_TAB:               btn = ImGuiKey_Tab;                                  break;
      case Input::BKEY_ENTER:             btn = ImGuiKey_Enter;                                break;
      case Input::BKEY_NUMLOCK:           btn = ImGuiKey_NumLock;                              break;
      case Input::BKEY_INSERT:            btn = ImGuiKey_Insert;                               break;
      case Input::BKEY_LALT:              btn = ImGuiKey_LeftAlt;                              break;
      case Input::BKEY_RALT:              btn = ImGuiKey_RightAlt;                             break;
      case Input::BKEY_CAPSLOCK:          btn = ImGuiKey_CapsLock;                             break;
      case Input::BKEY_ESCAPE:            btn = ImGuiKey_Escape;                               break;
      case Input::BKEY_SPACE:             btn = ImGuiKey_Space;           ch = ' ';            break;
      case Input::BKEY_PAGEUP:            btn = ImGuiKey_PageUp;                               break;
      case Input::BKEY_PAGEDOWN:          btn = ImGuiKey_PageDown;                             break;
      case Input::BKEY_END:               btn = ImGuiKey_End;                                  break;
      case Input::BKEY_HOME:              btn = ImGuiKey_Home;                                 break;
      case Input::BKEY_LEFT:              btn = ImGuiKey_LeftArrow;                            break;
      case Input::BKEY_UP:                btn = ImGuiKey_UpArrow;                              break;
      case Input::BKEY_RIGHT:             btn = ImGuiKey_RightArrow;                           break;
      case Input::BKEY_DOWN:              btn = ImGuiKey_DownArrow;                            break;
      case Input::BKEY_PRINTSCREEN:       btn = ImGuiKey_PrintScreen;                          break;
      case Input::BKEY_LCTRL:             btn = ImGuiKey_LeftCtrl;                             break;
      case Input::BKEY_RCTRL:             btn = ImGuiKey_RightCtrl;                            break;
      case Input::BKEY_DELETE:            btn = ImGuiKey_Delete;                               break;
      case Input::BKEY_0:                 btn = ImGuiKey_0;               ch = toCh('0', ')'); break;
      case Input::BKEY_1:                 btn = ImGuiKey_1;               ch = toCh('1', '!'); break;
      case Input::BKEY_2:                 btn = ImGuiKey_2;               ch = toCh('2', '@'); break;
      case Input::BKEY_3:                 btn = ImGuiKey_3;               ch = toCh('3', '#'); break;
      case Input::BKEY_4:                 btn = ImGuiKey_4;               ch = toCh('4', '$'); break;
      case Input::BKEY_5:                 btn = ImGuiKey_5;               ch = toCh('5', '%'); break;
      case Input::BKEY_6:                 btn = ImGuiKey_6;               ch = toCh('6', '^'); break;
      case Input::BKEY_7:                 btn = ImGuiKey_7;               ch = toCh('7', '&'); break;
      case Input::BKEY_8:                 btn = ImGuiKey_8;               ch = toCh('8', '*'); break;
      case Input::BKEY_9:                 btn = ImGuiKey_9;               ch = toCh('9', '('); break;
      case Input::BKEY_A:                 btn = ImGuiKey_A;               ch = toCh('a', 'A'); break;
      case Input::BKEY_B:                 btn = ImGuiKey_B;               ch = toCh('b', 'B'); break;
      case Input::BKEY_C:                 btn = ImGuiKey_C;               ch = toCh('c', 'C'); break;
      case Input::BKEY_D:                 btn = ImGuiKey_D;               ch = toCh('d', 'D'); break;
      case Input::BKEY_E:                 btn = ImGuiKey_E;               ch = toCh('e', 'E'); break;
      case Input::BKEY_F:                 btn = ImGuiKey_F;               ch = toCh('f', 'F'); break;
      case Input::BKEY_G:                 btn = ImGuiKey_G;               ch = toCh('g', 'G'); break;
      case Input::BKEY_H:                 btn = ImGuiKey_H;               ch = toCh('h', 'H'); break;
      case Input::BKEY_I:                 btn = ImGuiKey_I;               ch = toCh('i', 'I'); break;
      case Input::BKEY_J:                 btn = ImGuiKey_J;               ch = toCh('j', 'J'); break;
      case Input::BKEY_K:                 btn = ImGuiKey_K;               ch = toCh('k', 'K'); break;
      case Input::BKEY_L:                 btn = ImGuiKey_L;               ch = toCh('l', 'L'); break;
      case Input::BKEY_M:                 btn = ImGuiKey_M;               ch = toCh('m', 'M'); break;
      case Input::BKEY_N:                 btn = ImGuiKey_N;               ch = toCh('n', 'N'); break;
      case Input::BKEY_O:                 btn = ImGuiKey_O;               ch = toCh('o', 'O'); break;
      case Input::BKEY_P:                 btn = ImGuiKey_P;               ch = toCh('p', 'P'); break;
      case Input::BKEY_Q:                 btn = ImGuiKey_Q;               ch = toCh('q', 'Q'); break;
      case Input::BKEY_R:                 btn = ImGuiKey_R;               ch = toCh('r', 'R'); break;
      case Input::BKEY_S:                 btn = ImGuiKey_S;               ch = toCh('s', 'S'); break;
      case Input::BKEY_T:                 btn = ImGuiKey_T;               ch = toCh('t', 'T'); break;
      case Input::BKEY_U:                 btn = ImGuiKey_U;               ch = toCh('u', 'U'); break;
      case Input::BKEY_V:                 btn = ImGuiKey_V;               ch = toCh('v', 'V'); break;
      case Input::BKEY_W:                 btn = ImGuiKey_W;               ch = toCh('w', 'W'); break;
      case Input::BKEY_X:                 btn = ImGuiKey_X;               ch = toCh('x', 'X'); break;
      case Input::BKEY_Y:                 btn = ImGuiKey_Y;               ch = toCh('y', 'Y'); break;
      case Input::BKEY_Z:                 btn = ImGuiKey_Z;               ch = toCh('z', 'Z'); break;
      case Input::BKEY_LWIN:              btn = ImGuiKey_LeftSuper;                            break;
      case Input::BKEY_RWIN:              btn = ImGuiKey_RightSuper;                           break;
      case Input::BKEY_LSHIFT:            btn = ImGuiKey_LeftShift;                            break;
      case Input::BKEY_RSHIFT:            btn = ImGuiKey_RightShift;                           break;
      case Input::BKEY_NUMPAD0:           btn = ImGuiKey_Keypad0;                              break;
      case Input::BKEY_NUMPAD1:           btn = ImGuiKey_Keypad1;                              break;
      case Input::BKEY_NUMPAD2:           btn = ImGuiKey_Keypad2;                              break;
      case Input::BKEY_NUMPAD3:           btn = ImGuiKey_Keypad3;                              break;
      case Input::BKEY_NUMPAD4:           btn = ImGuiKey_Keypad4;                              break;
      case Input::BKEY_NUMPAD5:           btn = ImGuiKey_Keypad5;                              break;
      case Input::BKEY_NUMPAD6:           btn = ImGuiKey_Keypad6;                              break;
      case Input::BKEY_NUMPAD7:           btn = ImGuiKey_Keypad7;                              break;
      case Input::BKEY_NUMPAD8:           btn = ImGuiKey_Keypad8;                              break;
      case Input::BKEY_NUMPAD9:           btn = ImGuiKey_Keypad9;                              break;
      case Input::BKEY_NUMPAD_MULTIPLY:   btn = ImGuiKey_KeypadMultiply;                       break;
      case Input::BKEY_NUMPAD_ADD:        btn = ImGuiKey_KeypadAdd;                            break;
      case Input::BKEY_NUMPAD_SUBTRACT:   btn = ImGuiKey_KeypadSubtract;                       break;
      case Input::BKEY_NUMPAD_DECIMAL:    btn = ImGuiKey_KeypadDecimal;                        break;
      case Input::BKEY_NUMPAD_DIVIDE:     btn = ImGuiKey_KeypadDivide;                         break;
      case Input::BKEY_F1:                btn = ImGuiKey_F1;                                   break;
      case Input::BKEY_F2:                btn = ImGuiKey_F2;                                   break;
      case Input::BKEY_F3:                btn = ImGuiKey_F3;                                   break;
      case Input::BKEY_F4:                btn = ImGuiKey_F4;                                   break;
      case Input::BKEY_F5:                btn = ImGuiKey_F5;                                   break;
      case Input::BKEY_F6:                btn = ImGuiKey_F6;                                   break;
      case Input::BKEY_F7:                btn = ImGuiKey_F7;                                   break;
      case Input::BKEY_F8:                btn = ImGuiKey_F8;                                   break;
      case Input::BKEY_F9:                btn = ImGuiKey_F9;                                   break;
      case Input::BKEY_F10:               btn = ImGuiKey_F10;                                  break;
      case Input::BKEY_F11:               btn = ImGuiKey_F11;                                  break;
      case Input::BKEY_F12:               btn = ImGuiKey_F12;                                  break;
      case Input::BKEY_MINUS:             btn = ImGuiKey_Minus;           ch = toCh('-', '_'); break;
      case Input::BKEY_EQUAL:             btn = ImGuiKey_Equal;           ch = toCh('=', '+'); break;
      case Input::BKEY_COMMA:             btn = ImGuiKey_Comma;           ch = toCh(',', '<'); break;
      case Input::BKEY_PERIOD:            btn = ImGuiKey_Period;          ch = toCh('.', '>'); break;
      default: break;
    }

    if (btn != -1) [[unlikely]]
    {
      io.AddKeyEvent((ImGuiKey)btn, pressed);
      
      if (ch != -1 && pressed)
        io.AddInputCharacter(ch);
    }

    return Input::InputRouterProcessStatus::ProcessFurther;
  }
}
