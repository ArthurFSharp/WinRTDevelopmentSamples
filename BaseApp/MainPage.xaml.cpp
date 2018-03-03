//
// MainPage.xaml.cpp
// Implémentation de la classe MainPage.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "BasicPage.xaml.h"

using namespace BaseApp;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;

// Pour en savoir plus sur le modèle d'élément Page vierge, consultez la page http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new BaseApp::Common::LoadStateEventHandler(this, &BaseApp::MainPage::LoadState);
	navigationHelper->SaveState += ref new BaseApp::Common::SaveStateEventHandler(this, &BaseApp::MainPage::SaveState);
}

DependencyProperty^ BaseApp::MainPage::_navigationHelperProperty =
DependencyProperty::Register("NavigationHelper",
	TypeName(Common::NavigationHelper::typeid), TypeName(MainPage::typeid), nullptr);

void BaseApp::MainPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void BaseApp::MainPage::OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs ^ e) 
{
	NavigationHelper->OnNavigatedFrom(e);
}

void BaseApp::MainPage::LoadState(Platform::Object ^ sender, Common::LoadStateEventArgs^ e)
{
	(void)sender;
	(void)e;
}

void BaseApp::MainPage::SaveState(Platform::Object ^ sender, Common::SaveStateEventArgs ^ e)
{
	(void)sender;
	(void)e;
}

void BaseApp::MainPage::btnNavigate_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Frame->Navigate(TypeName(BasicPage::typeid));
}

Common::NavigationHelper^ BaseApp::MainPage::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}
