//
// NavigationHelper.cpp
// Implémentation de NavigationHelper et des classes associées
//

#include "pch.h"
#include "NavigationHelper.h"
#include "RelayCommand.h"
#include "SuspensionManager.h"

using namespace WinRTControls::Common;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
using namespace Windows::Phone::UI::Input;
#endif

/// <summary>
/// Initialise une nouvelle instance de la classe <see cref="LoadStateEventArgs"/>.
/// </summary>
/// <param name="navigationParameter">
/// Valeur de paramètre passée à <see cref="Frame->Navigate(Type, Object)"/> 
/// lors de la requête initiale de cette page.
/// </param>
/// <param name="pageState">
/// Dictionnaire d'état conservé par cette page durant une session
/// antérieure.  Null lors de la première visite de la page.
/// </param>
LoadStateEventArgs::LoadStateEventArgs(Object^ navigationParameter, IMap<String^, Object^>^ pageState)
{
	_navigationParameter = navigationParameter;
	_pageState = pageState;
}

/// <summary>
/// Obtient la propriété <see cref="NavigationParameter"/> de la classe <see cref"LoadStateEventArgs"/>.
/// </summary>
Object^ LoadStateEventArgs::NavigationParameter::get()
{
	return _navigationParameter;
}

/// <summary>
/// Obtient la propriété <see cref="PageState"/> de la classe <see cref"LoadStateEventArgs"/>.
/// </summary>
IMap<String^, Object^>^ LoadStateEventArgs::PageState::get()
{
	return _pageState;
}

/// <summary>
/// Initialise une nouvelle instance de la classe <see cref="SaveStateEventArgs"/>.
/// </summary>
/// <param name="pageState">Dictionnaire vide à remplir à l'aide de l'état sérialisable.</param>
SaveStateEventArgs::SaveStateEventArgs(IMap<String^, Object^>^ pageState)
{
	_pageState = pageState;
}

/// <summary>
/// Obtient la propriété <see cref="PageState"/> de la classe <see cref"SaveStateEventArgs"/>.
/// </summary>
IMap<String^, Object^>^ SaveStateEventArgs::PageState::get()
{
	return _pageState;
}

/// <summary>
/// Initialise une nouvelle instance de la classe <see cref="NavigationHelper"/>.
/// </summary>
/// <param name="page">Référence à la page active utilisée pour la navigation.  
/// Cette référence permet de manipuler le frame et de garantir que le clavier 
/// les requêtes de navigation ont lieu uniquement lorsque la page occupe toute la fenêtre.</param>
NavigationHelper::NavigationHelper(Page^ page, RelayCommand^ goBack, RelayCommand^ goForward) :
_page(page),
_goBackCommand(goBack),
_goForwardCommand(goForward)
{
	// Lorsque cette page fait partie de l'arborescence d'éléments visuels, effectue deux modifications :
	// 1) Mappe l'état d'affichage de l'application à l'état visuel pour la page
	// 2) Gérer les raccourcis de demande de navigation physique
	_loadedEventToken = page->Loaded += ref new RoutedEventHandler(this, &NavigationHelper::OnLoaded);

	//// Annuler les mêmes modifications lorsque la page n'est plus visible
	_unloadedEventToken = page->Unloaded += ref new RoutedEventHandler(this, &NavigationHelper::OnUnloaded);
}

NavigationHelper::~NavigationHelper()
{
	delete _goBackCommand;
	delete _goForwardCommand;

	_page = nullptr;
}

/// <summary>
/// Appelé lorsque la page appartient à une arborescence d'éléments visuels
/// </summary>
/// <param name="sender">Instance qui a déclenché l'événement.</param>
/// <param name="e">Données d'événement décrivant les conditions ayant déclenché l'événement.</param>
void NavigationHelper::OnLoaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
	_backPressedEventToken = HardwareButtons::BackPressed +=
		ref new EventHandler<BackPressedEventArgs^>(this,
		&NavigationHelper::HardwareButton_BackPressed);
#else
	Page ^page = _page.Resolve<Page>();

	// La navigation à l'aide du clavier et de la souris s'applique uniquement lorsque la totalité de la fenêtre est occupée
	if (page != nullptr &&
		page->ActualHeight == Window::Current->Bounds.Height &&
		page->ActualWidth == Window::Current->Bounds.Width)
	{
		// Écoute directement la fenêtre, ce qui ne requiert pas le focus
		_acceleratorKeyEventToken = Window::Current->CoreWindow->Dispatcher->AcceleratorKeyActivated +=
			ref new TypedEventHandler<CoreDispatcher^, AcceleratorKeyEventArgs^>(this,
			&NavigationHelper::CoreDispatcher_AcceleratorKeyActivated);

		_pointerPressedEventToken = Window::Current->CoreWindow->PointerPressed +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this,
			&NavigationHelper::CoreWindow_PointerPressed);

		_navigationShortcutsRegistered = true;
	}
#endif
}

/// <summary>
/// Appelé lorsque la page est supprimée de l'arborescence d'éléments visuels
/// </summary>
/// <param name="sender">Instance qui a déclenché l'événement.</param>
/// <param name="e">Données d'événement décrivant les conditions ayant déclenché l'événement.</param>
void NavigationHelper::OnUnloaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
	HardwareButtons::BackPressed -= _backPressedEventToken;
#else
	if (_navigationShortcutsRegistered)
	{
		Window::Current->CoreWindow->Dispatcher->AcceleratorKeyActivated -= _acceleratorKeyEventToken;
		Window::Current->CoreWindow->PointerPressed -= _pointerPressedEventToken;
		_navigationShortcutsRegistered = false;
	}
#endif

	// Supprimer le gestionnaire et libérer la référence à la page
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		page->Loaded -= _loadedEventToken;
		page->Unloaded -= _unloadedEventToken;
		delete _goBackCommand;
		delete _goForwardCommand;
		_goForwardCommand = nullptr;
		_goBackCommand = nullptr;
	}
}

#pragma region Navigation support

/// <summary>
/// Méthode utilisée par la propriété <see cref="GoBackCommand"/>
/// pour déterminer si le <see cref="Frame"/> peut reculer.
/// </summary>
/// <returns>
/// true si le <see cref="Frame"/> possède au moins une entrée 
/// dans l'historique de navigation vers l'arrière.
/// </returns>
bool NavigationHelper::CanGoBack()
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frame = page->Frame;
		return (frame != nullptr && frame->CanGoBack);
	}

	return false;
}

/// <summary>
/// Méthode utilisée par la propriété <see cref="GoBackCommand"/>
/// pour appeler la méthode <see cref="Windows::UI::Xaml::Controls::Frame::GoBack"/>.
/// </summary>
void NavigationHelper::GoBack()
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frame = page->Frame;
		if (frame != nullptr && frame->CanGoBack)
		{
			frame->GoBack();
		}
	}
}

/// <summary>
/// Méthode utilisée par la propriété <see cref="GoForwardCommand"/>
/// pour déterminer si le <see cref="Frame"/> peut avancer.
/// </summary>
/// <returns>
/// true si le <see cref="Frame"/> possède au moins une entrée 
/// dans l'historique de navigation vers l'avant.
/// </returns>
bool NavigationHelper::CanGoForward()
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frame = page->Frame;
		return (frame != nullptr && frame->CanGoForward);
	}

	return false;
}

/// <summary>
/// Méthode utilisée par la propriété <see cref="GoForwardCommand"/>
/// pour appeler la méthode <see cref="Windows::UI::Xaml::Controls::Frame::GoBack"/>.
/// </summary>
void NavigationHelper::GoForward()
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frame = page->Frame;
		if (frame != nullptr && frame->CanGoForward)
		{
			frame->GoForward();
		}
	}
}

/// <summary>
/// Obtient la propriété <see cref="GoBackCommand"/> de la classe <see cref"NavigationHelper"/>.
/// </summary>
RelayCommand^ NavigationHelper::GoBackCommand::get()
{
	if (_goBackCommand == nullptr)
	{
		_goBackCommand = ref new RelayCommand(
			[this](Object^) -> bool
		{
			return CanGoBack();
		},
			[this](Object^) -> void
		{
			GoBack();
		}
		);
	}
	return _goBackCommand;
}

/// <summary>
/// Obtient la propriété <see cref="GoForwardCommand"/> de la classe <see cref"NavigationHelper"/>.
/// </summary>
RelayCommand^ NavigationHelper::GoForwardCommand::get()
{
	if (_goForwardCommand == nullptr)
	{
		_goForwardCommand = ref new RelayCommand(
			[this](Object^) -> bool
		{
			return CanGoForward();
		},
			[this](Object^) -> void
		{
			GoForward();
		}
		);
	}
	return _goForwardCommand;
}

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
/// <summary>
/// Gère l'appui sur le bouton Précédente et parcourt l'historique du frame racine.
/// </summary>
void NavigationHelper::HardwareButton_BackPressed(Object^ sender, BackPressedEventArgs^ e)
{
	if (this->GoBackCommand->CanExecute(nullptr))
	{
		e->Handled = true;
		this->GoBackCommand->Execute(nullptr);
	}
}
#else
/// <summary>
/// Invoqué à chaque séquence de touches, notamment les touches système comme les combinaisons utilisant la touche Alt, lorsque
/// cette page est active et occupe la totalité de la fenêtre.  Utilisé pour détecter la navigation à l'aide du clavier
/// entre les pages, même lorsque la page elle-même n'a pas le focus.
/// </summary>
/// <param name="sender">Instance qui a déclenché l'événement.</param>
/// <param name="e">Données d'événement décrivant les conditions ayant déclenché l'événement.</param>
void NavigationHelper::CoreDispatcher_AcceleratorKeyActivated(CoreDispatcher^ sender,
	AcceleratorKeyEventArgs^ e)
{
	sender; // Paramètre non utilisé
	auto virtualKey = e->VirtualKey;

	// Approfondit les recherches uniquement lorsque les touches Gauche, Droite ou les touches Précédent et Suivant dédiées
	// sont actionnées
	if ((e->EventType == CoreAcceleratorKeyEventType::SystemKeyDown ||
		e->EventType == CoreAcceleratorKeyEventType::KeyDown) &&
		(virtualKey == VirtualKey::Left || virtualKey == VirtualKey::Right ||
		virtualKey == VirtualKey::GoBack || virtualKey == VirtualKey::GoForward))
	{
		auto coreWindow = Window::Current->CoreWindow;
		auto downState = Windows::UI::Core::CoreVirtualKeyStates::Down;
		bool menuKey = (coreWindow->GetKeyState(VirtualKey::Menu) & downState) == downState;
		bool controlKey = (coreWindow->GetKeyState(VirtualKey::Control) & downState) == downState;
		bool shiftKey = (coreWindow->GetKeyState(VirtualKey::Shift) & downState) == downState;
		bool noModifiers = !menuKey && !controlKey && !shiftKey;
		bool onlyAlt = menuKey && !controlKey && !shiftKey;

		if ((virtualKey == VirtualKey::GoBack && noModifiers) ||
			(virtualKey == VirtualKey::Left && onlyAlt))
		{
			// Lorsque la touche Précédent ou les touches Alt+Gauche sont actionnées, navigue vers l'arrière
			e->Handled = true;
			GoBackCommand->Execute(this);
		}
		else if ((virtualKey == VirtualKey::GoForward && noModifiers) ||
			(virtualKey == VirtualKey::Right && onlyAlt))
		{
			// Lorsque la touche Suivant ou les touches Alt+Droite sont actionnées, navigue vers l'avant
			e->Handled = true;
			GoForwardCommand->Execute(this);
		}
	}
}

/// <summary>
/// Invoqué sur chaque clic de souris, pression d'écran tactile ou interaction équivalente lorsque cette
/// page est active et occupe la totalité de la fenêtre.  Utilisé pour détecter les clics de souris Suivant et Précédent
/// de style navigateur pour naviguer entre les pages.
/// </summary>
/// <param name="sender">Instance qui a déclenché l'événement.</param>
/// <param name="e">Données d'événement décrivant les conditions ayant déclenché l'événement.</param>
void NavigationHelper::CoreWindow_PointerPressed(CoreWindow^ sender, PointerEventArgs^ e)
{
	auto properties = e->CurrentPoint->Properties;

	// Ignore les pressions simultanées sur les boutons droit, gauche et central
	if (properties->IsLeftButtonPressed ||
		properties->IsRightButtonPressed ||
		properties->IsMiddleButtonPressed)
	{
		return;
	}

	// Si les boutons Précédent ou Suivant sont utilisés (mais pas les deux à la fois), navigue de manière appropriée
	bool backPressed = properties->IsXButton1Pressed;
	bool forwardPressed = properties->IsXButton2Pressed;
	if (backPressed ^ forwardPressed)
	{
		e->Handled = true;
		if (backPressed)
		{
			if (GoBackCommand->CanExecute(this))
			{
				GoBackCommand->Execute(this);
			}
		}
		else
		{
			if (GoForwardCommand->CanExecute(this))
			{
				GoForwardCommand->Execute(this);
			}
		}
	}
}
#endif

#pragma endregion

#pragma region Process lifetime management

/// <summary>
/// Invoqué lorsque cette page est sur le point d'être affichée dans un frame.
/// </summary>
/// <param name="e">Données d'événement décrivant la manière dont l'utilisateur a accédé à cette page.  La propriété Parameter
/// fournit le groupe devant être affiché.</param>
void NavigationHelper::OnNavigatedTo(NavigationEventArgs^ e)
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frameState = SuspensionManager::SessionStateForFrame(page->Frame);
		_pageKey = "Page-" + page->Frame->BackStackDepth;

		if (e->NavigationMode == NavigationMode::New)
		{
			// Efface l'état existant pour la navigation avant lors de l'ajout d'une nouvelle page à la
			// pile de navigation
			auto nextPageKey = _pageKey;
			int nextPageIndex = page->Frame->BackStackDepth;
			while (frameState->HasKey(nextPageKey))
			{
				frameState->Remove(nextPageKey);
				nextPageIndex++;
				nextPageKey = "Page-" + nextPageIndex;
			}

			// Passe le paramètre de navigation à la nouvelle page
			LoadState(this, ref new LoadStateEventArgs(e->Parameter, nullptr));
		}
		else
		{
			// Passe le paramètre de navigation et conserve l'état de page de la page, en utilisant
			// la même stratégie pour charger l'état suspendu et recréer les pages supprimées
			// du cache
			LoadState(this, ref new LoadStateEventArgs(e->Parameter, safe_cast<IMap<String^, Object^>^>(frameState->Lookup(_pageKey))));
		}
	}
}

/// <summary>
/// Invoqué lorsque cette page n'est plus affichée dans un frame.
/// </summary>
/// <param name="e">Données d'événement décrivant la manière dont l'utilisateur a accédé à cette page.  La propriété Parameter
/// fournit le groupe devant être affiché.</param>
void NavigationHelper::OnNavigatedFrom(NavigationEventArgs^ e)
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frameState = SuspensionManager::SessionStateForFrame(page->Frame);
		auto pageState = ref new Map<String^, Object^>();
		SaveState(this, ref new SaveStateEventArgs(pageState));
		frameState->Insert(_pageKey, pageState);
	}
}
#pragma endregion