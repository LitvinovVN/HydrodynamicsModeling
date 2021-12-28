using Microsoft.AspNetCore.Components;
using ModelingSystem.Client.Helpers;
using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Client.Pages.Clusters
{
    public partial class Clusters
    {
        [Inject] IRepository repository { get; set; }


        private List<Cluster> clusters;

        protected async override Task OnInitializedAsync()
        {
            await Task.Delay(2000);
            clusters = repository.GetClusters();
        }

        private void AddNewCluster()
        {
            clusters.Add(new Cluster { Guid = new Guid(), Name = clusters.Count.ToString(), Caption = clusters.Count.ToString() + "caption" });
        }
    }
}
