using Microsoft.JSInterop;

namespace ModelingSystem.Client.Helpers
{
    public class DisplayMessage: IDisplayMessage
    {
        private readonly IJSRuntime js;

        public DisplayMessage(IJSRuntime js)
        {
            this.js = js;
        }

        public async ValueTask DisplayErrorMessage(string message)
        {
            await DoDisplayMessage("Ошибка!", message, "error");
        }

        public async ValueTask DisplaySuccessMessage(string message)
        {
            await DoDisplayMessage("Выполнено!", message, "success");
        }

        private async ValueTask DoDisplayMessage(string title, string message, string messageType)
        {
            await js.InvokeVoidAsync("Swal.fire", title, message, messageType);
        }
    }
}
