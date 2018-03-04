//
// AppBarPage.xaml.cpp
// Implémentation de la classe AppBarPage
//

#include "pch.h"
#include "AppBarPage.xaml.h"

using namespace WinRTControls;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// Pour en savoir plus sur le modèle d'élément Page de base, consultez la page http://go.microsoft.com/fwlink/?LinkId=234237

AppBarPage::AppBarPage()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Map<String^,Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &AppBarPage::LoadState);
	navigationHelper->SaveState += ref new Common::SaveStateEventHandler(this, &AppBarPage::SaveState);
}

DependencyProperty^ AppBarPage::_defaultViewModelProperty =
	DependencyProperty::Register("DefaultViewModel",
		TypeName(IObservableMap<String^,Object^>::typeid), TypeName(AppBarPage::typeid), nullptr);

/// <summary>
/// utilisée en tant que modèle d'affichage trivial.
/// </summary>
IObservableMap<String^, Object^>^ AppBarPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ AppBarPage::_navigationHelperProperty =
	DependencyProperty::Register("NavigationHelper",
		TypeName(Common::NavigationHelper::typeid), TypeName(AppBarPage::typeid), nullptr);

/// <summary>
/// Obtient une implémentation de <see cref="NavigationHelper"/> conçue pour être
/// utilisée en tant que modèle d'affichage trivial.
/// </summary>
Common::NavigationHelper^ AppBarPage::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

#pragma region Navigation support

/// Les méthodes fournies dans cette section sont utilisées simplement pour permettre
/// NavigationHelper pour répondre aux méthodes de navigation de la page.
/// 
/// La logique spécifique à la page doit être placée dans les gestionnaires d'événements pour  
/// <see cref="NavigationHelper::LoadState"/>
/// et <see cref="NavigationHelper::SaveState"/>.
/// Le paramètre de navigation est disponible dans la méthode LoadState 
/// en plus de l'état de page conservé durant une session antérieure.

void AppBarPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void AppBarPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);
}

#pragma endregion

/// <summary>
/// Remplit la page à l'aide du contenu passé lors de la navigation. Tout état enregistré est également
/// fourni lorsqu'une page est recréée à partir d'une session antérieure.
/// </summary>
/// <param name="sender">
/// La source de l'événement ; en général <see cref="NavigationHelper"/>
/// </param>
/// <param name="e">Données d'événement qui fournissent le paramètre de navigation transmis à
/// <see cref="Frame.Navigate(Type, Object)"/> lors de la requête initiale de cette page et
/// un dictionnaire d'état conservé par cette page durant une session
/// antérieure. L'état n'aura pas la valeur Null lors de la première visite de la page.</param>
void AppBarPage::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	(void) sender;	// Paramètre non utilisé
	(void) e;	// Paramètre non utilisé
}

/// <summary>
/// Conserve l'état associé à cette page en cas de suspension de l'application ou de
/// suppression de la page du cache de navigation. Les valeurs doivent être conformes aux
/// exigences de sérialisation de <see cref="SuspensionManager::SessionState"/>.
/// </summary>
/// <param name="sender">La source de l'événement ; en général <see cref="NavigationHelper"/></param>
/// <param name="e">Données d'événement qui fournissent un dictionnaire vide à remplir à l'aide de
/// état sérialisable.</param>
void AppBarPage::SaveState(Object^ sender, Common::SaveStateEventArgs^ e){
	(void) sender;	// Paramètre non utilisé
	(void) e; // Paramètre non utilisé
}
