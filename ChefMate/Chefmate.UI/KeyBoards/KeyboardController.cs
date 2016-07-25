using Chefmate.Core.Enums;

namespace Chefmate.UI.KeyBoards
{
    public class KeyboardController
    {

        private static KeyboardController _instance;
        private static volatile object _synRoot = new object();
        private KeyboardController()
        {
        }

        public static KeyboardController Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_synRoot)
                    {
                        if (_instance == null)
                            _instance = new KeyboardController();
                    }
                }
                return _instance;
            }
        }

        public string OpenNumPad(string text, NumpadMode numpadMode)
        {
            var retVal = text;
            var keyboard = new NumKeyboard();
            keyboard.Topmost = true;
            keyboard.SetMode(numpadMode);
            keyboard.InitializeParameters(text);
            keyboard.ShowDialog();
            if (keyboard.ModalResult)
            {
                retVal = keyboard.GetResult().Trim();
            }
            return retVal;
        }

        public string OpenKeyBoard(string header, string text)
        {
            var retVal = text;
            var keyboard = new TextKeyBoard();
            keyboard.Topmost = true;
            keyboard.InitializeParameters(header, text);
            keyboard.ShowDialog();
            if (keyboard.ModalResult)
            {
                retVal = keyboard.GetResult().Trim();
            }
            return retVal;
        }
    }
}