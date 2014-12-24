#pragma once

//Reserved class keys: 0x0101 - 0x0150

#include <SCFStandard.h>

#define SCF_USER_INTERFACE_1

#include "../Source/UserInterface/Message.h"
#include "../Source/UserInterface/Action.h"
#include "../Source/UserInterface/Event.h"

#include "../Source/UserInterface/EventKey.h"
#include "../Source/UserInterface/EventMouse.h"
#include "../Source/UserInterface/EventWindow.h"

#include "../Source/UserInterface/WindowAbstract.h"
#include "../Source/UserInterface/Window.h"

#include "../Source/UserInterface/Button.h"
#include "../Source/UserInterface/ButtonClick.h"

#include "../Source/UserInterface/ButtonMultiState.h"
#include "../Source/UserInterface/ButtonRadio.h"
#include "../Source/UserInterface/ButtonCheck.h"
#include "../Source/UserInterface/ButtonPush.h"

#include "../Source/UserInterface/Edit.h"
#include "../Source/UserInterface/Spinner.h"

#include "../Source/UserInterface/Builder.h"

#include "../Source/UserInterface/MessageLoop.h"
