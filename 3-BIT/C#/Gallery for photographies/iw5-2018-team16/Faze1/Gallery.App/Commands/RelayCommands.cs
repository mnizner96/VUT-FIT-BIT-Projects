using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Gallery.App.Commands
{
    public class RelayCommands : ICommand
    {
        private readonly Action<object> executeAction;
        private readonly Func<object, bool> canExecuteAction;

        public RelayCommands(Action<object> executeAction, Func<object, bool> canExecuteAction = null)
        {
            this.executeAction = executeAction;
            this.canExecuteAction = canExecuteAction;
        } 

        public RelayCommands(Action executeAction, Func<bool> canExecuteAction = null)
            : this(p => executeAction(), p => canExecuteAction?.Invoke() ?? true)
        {

        }

        public bool CanExecute(object parameter)
        {
            return canExecuteAction?.Invoke(parameter) ?? true;
        }

        public void Execute(object parameter)
        {
            executeAction?.Invoke(parameter);
        }

        public void RaiseCanExecuteChanged()
        {
            CanExecuteChanged?.Invoke(this, EventArgs.Empty);
        }

        public event EventHandler CanExecuteChanged;
    }
}
