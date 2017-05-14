using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(iepproj.Startup))]
namespace iepproj
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
            app.MapSignalR();
        }
    }
}
