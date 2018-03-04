//
// NavigationHelper.h
// Déclaration de NavigationHelper et des classes associées
//

#pragma once

#include "RelayCommand.h"

namespace WinRTControls
{
	namespace Common
	{
		/// <summary>
		/// Classe utilisée pour contenir les données d'événements requises lorsqu'une page tente de charger l'état.
		/// </summary>
		public ref class LoadStateEventArgs sealed
		{
		public:

			/// <summary>
			/// Valeur de paramètre passée à <see cref="Frame->Navigate(Type, Object)"/> 
			/// lors de la requête initiale de cette page.
			/// </summary>
			property Platform::Object^ NavigationParameter
			{
				Platform::Object^ get();
			}

			/// <summary>
			/// Dictionnaire d'état conservé par cette page durant une session
			/// antérieure.  Null lors de la première visite de la page.
			/// </summary>
			property Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ PageState
			{
				Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ get();
			}

		internal:
			LoadStateEventArgs(Platform::Object^ navigationParameter, Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState);

		private:
			Platform::Object^ _navigationParameter;
			Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ _pageState;
		};

		/// <summary>
		/// Représente la méthode qui gérera l'événement <see cref="NavigationHelper->LoadState"/>
		/// </summary>
		public delegate void LoadStateEventHandler(Platform::Object^ sender, LoadStateEventArgs^ e);

		/// <summary>
		/// Classe utilisée pour contenir les données d'événements requises lorsqu'une page tente d'enregistrer l'état.
		/// </summary>
		public ref class SaveStateEventArgs sealed
		{
		public:

			/// <summary>
			/// Dictionnaire vide à remplir à l'aide de l'état sérialisable.
			/// </summary>
			property Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ PageState
			{
				Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ get();
			}

		internal:
			SaveStateEventArgs(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState);

		private:
			Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ _pageState;
		};

		/// <summary>
		/// Représente la méthode qui gérera l'événement <see cref="NavigationHelper->SaveState"/>
		/// </summary>
		public delegate void SaveStateEventHandler(Platform::Object^ sender, SaveStateEventArgs^ e);

		/// <summary>
		/// NavigationHelper aide à naviguer entre les pages.  Il fournit les commandes utilisées pour 
		/// naviguer vers l'arrière et l'avant ainsi que des registres pour les raccourcis de souris et de clavier 
		/// utilisés pour se déplacer vers l'avant ou vers l'arrière dans Windows et le bouton Retour physique dans
		/// Windows Phone. De plus, il intègre SuspensionManger pour assurer la gestion de la durée de vie du processus
		/// et la gestion de l’état lors de la navigation entre pages.
		/// </summary>
		/// <example>
		/// Pour utiliser NavigationHelper, suivez les deux étapes ci-dessous ou
		/// commencer par BasicPage ou un autre modèle d'élément Page différent de BlankPage.
		/// 
		/// 1) Créer une instance de NavigationHelper à un emplacement tel que 
		///		constructeur de la page et inscrire un rappel pour LoadState et 
		///		événements SaveState.
		/// <code>
		///		MyPage::MyPage()
		///		{
		///			InitializeComponent();
		///			auto navigationHelper = ref new Common::NavigationHelper(this);
		///			navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &MyPage::LoadState);
		///			navigationHelper->SaveState += ref new Common::SaveStateEventHandler(this, &MyPage::SaveState);
		///		}
		///		
		///		void MyPage::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
		///		{ }
		///		void MyPage::SaveState(Object^ sender, Common::SaveStateEventArgs^ e)
		///		{ }
		/// </code>
		/// 
		/// 2) Inscrire la page pour appeler NavigationHelper lorsque la page participe 
		///		à la navigation en remplaçant les événements <see cref="Windows::UI::Xaml::Controls::Page::OnNavigatedTo"/> 
		///		et <see cref="Windows::UI::Xaml::Controls::Page::OnNavigatedFrom"/>.
		/// <code>
		///		void MyPage::OnNavigatedTo(NavigationEventArgs^ e)
		///		{
		///			NavigationHelper->OnNavigatedTo(e);
		///		}
		///
		///		void MyPage::OnNavigatedFrom(NavigationEventArgs^ e)
		///		{
		///			NavigationHelper->OnNavigatedFrom(e);
		///		}
		/// </code>
		/// </example>
		[Windows::Foundation::Metadata::WebHostHidden]
		[Windows::UI::Xaml::Data::Bindable]
		public ref class NavigationHelper sealed
		{
		public:
			/// <summary>
			/// <see cref="RelayCommand"/> utilisée pour la liaison à la propriété Command du bouton Précédent
			/// pour accéder à l'élément le plus récent de l'historique de navigation vers l'arrière, si un frame
			/// gère son propre historique de navigation.
			/// 
			/// La commande<see cref="RelayCommand"/> est configurée pour utiliser la méthode virtuelle <see cref="GoBack"/>
			/// comme action d'exécution et <see cref="CanGoBack"/> pour CanExecute.
			/// </summary>
			property RelayCommand^ GoBackCommand
			{
				RelayCommand^ get();
			}

			/// <summary>
			/// <see cref="RelayCommand"/> utilisée pour accéder à l'élément le plus récent du 
			/// l'historique de navigation avant, si un frame gère son propre historique de navigation.
			/// 
			/// La commande<see cref="RelayCommand"/> est configurée pour utiliser la méthode virtuelle <see cref="GoForward"/>
			/// comme action d'exécution et <see cref="CanGoForward"/> pour CanExecute.
			/// </summary>
			property RelayCommand^ GoForwardCommand
			{
				RelayCommand^ get();
			}

		internal:
			NavigationHelper(Windows::UI::Xaml::Controls::Page^ page,
				RelayCommand^ goBack = nullptr,
				RelayCommand^ goForward = nullptr);

			bool CanGoBack();
			void GoBack();
			bool CanGoForward();
			void GoForward();

			void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e);
			void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e);

			event LoadStateEventHandler^ LoadState;
			event SaveStateEventHandler^ SaveState;

		private:
			Platform::WeakReference _page;

			RelayCommand^ _goBackCommand;
			RelayCommand^ _goForwardCommand;

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
			Windows::Foundation::EventRegistrationToken _backPressedEventToken;

			void HardwareButton_BackPressed(Platform::Object^ sender,
				Windows::Phone::UI::Input::BackPressedEventArgs^ e);
#else
			bool _navigationShortcutsRegistered;
			Windows::Foundation::EventRegistrationToken _acceleratorKeyEventToken;
			Windows::Foundation::EventRegistrationToken _pointerPressedEventToken;

			void CoreDispatcher_AcceleratorKeyActivated(Windows::UI::Core::CoreDispatcher^ sender,
				Windows::UI::Core::AcceleratorKeyEventArgs^ e);
			void CoreWindow_PointerPressed(Windows::UI::Core::CoreWindow^ sender,
				Windows::UI::Core::PointerEventArgs^ e);
#endif

			Platform::String^ _pageKey;
			Windows::Foundation::EventRegistrationToken _loadedEventToken;
			Windows::Foundation::EventRegistrationToken _unloadedEventToken;
			void OnLoaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void OnUnloaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

			~NavigationHelper();
		};
	}
}
