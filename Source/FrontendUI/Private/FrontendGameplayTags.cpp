// Dmytro Chesniuk All Rights Reserved


#include "FrontendGameplayTags.h"

namespace FrontendGameplayTags
{
	// <--- Frontend Widget Stack --->
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Modal, "Frontend.WidgetStack.Modal")
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameMenu, "Frontend.WidgetStack.GameMenu")
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameHud, "Frontend.WidgetStack.GameHud")
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Frontend, "Frontend.WidgetStack.Frontend")

	// <--- Frontend Widgets --->
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_PressAnyKeyScreen, "Frontend.Widget.PressAnyKeyScreen")
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_MainMenuScreen, "Frontend.Widget.MainMenuScreen")
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_ConfirmScreen, "Frontend.Widget.ConfirmScreen")
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_StoryScreen, "Frontend.Widget.StoryScreen")
}