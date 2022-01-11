namespace ModelingSystem.Client.Helpers
{
    public interface IDisplayMessage
    {
        ValueTask DisplayErrorMessage(string message);
        ValueTask DisplaySuccessMessage(string message);
    }
}
