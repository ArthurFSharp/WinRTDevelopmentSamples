//
// RelayCommand.h
// Déclaration de RelayCommand et des classes associées
//

#pragma once

// <summary>
// Commande dont le seul objectif est de relayer sa fonctionnalité 
// à d'autres objets en appelant des délégués. 
// La valeur de retour par défaut pour la méthode CanExecute est 'true'.
// <see cref="RaiseCanExecuteChanged"/> doit être appelé lorsque
// <see cref="CanExecute"/> est censé retourner une valeur différente.
// </summary>


namespace WinRTControls
{
	namespace Common
	{
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class RelayCommand sealed :[Windows::Foundation::Metadata::Default] Windows::UI::Xaml::Input::ICommand
		{
		public:
			virtual event Windows::Foundation::EventHandler<Object^>^ CanExecuteChanged;
			virtual bool CanExecute(Object^ parameter);
			virtual void Execute(Object^ parameter);
			virtual ~RelayCommand();

		internal:
			RelayCommand(std::function<bool(Platform::Object^)> canExecuteCallback,
				std::function<void(Platform::Object^)> executeCallback);
			void RaiseCanExecuteChanged();

		private:
			std::function<bool(Platform::Object^)> _canExecuteCallback;
			std::function<void(Platform::Object^)> _executeCallback;
		};
	}
}