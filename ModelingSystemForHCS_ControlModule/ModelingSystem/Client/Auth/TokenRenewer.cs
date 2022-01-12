

using System.Timers;

namespace ModelingSystem.Client.Auth
{
    public class TokenRenewer : IDisposable
    {        
        System.Timers.Timer timer;
        private readonly ILoginService loginService;

        public TokenRenewer(ILoginService loginService)
        {
            this.loginService = loginService;
        }

        public void Initiate()
        {
            timer = new System.Timers.Timer();
            timer.Interval = 5000; //1000 * 60 * 4;// 4 minutes
            timer.Elapsed += Timer_Elapsed;
            timer.Start();
        }

        private void Timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            loginService.TryRenewToken();
        }

        public void Dispose()
        {
            timer?.Dispose();
        }
    }
}
