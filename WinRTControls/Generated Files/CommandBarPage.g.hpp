﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "CommandBarPage.xaml.h"




void ::WinRTControls::CommandBarPage::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///CommandBarPage.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///CommandBarPage.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the Button named 'backButton'
    backButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"backButton"));
    // Get the TextBlock named 'pageTitle'
    pageTitle = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"pageTitle"));
}

void ::WinRTControls::CommandBarPage::Connect(int connectionId, Platform::Object^ target)
{
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}

