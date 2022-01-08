using Microsoft.AspNetCore.Components;
using Microsoft.AspNetCore.Components.Authorization;
using ModelingSystem.Client.Helpers;
using ModelingSystem.Client.Repository;
using ModelingSystem.Shared.DTOs;
using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Client.Pages.Clusters
{
    public partial class Clusters
    {
        [Inject] IClusterRepository clusterRepository { get; set; }

        [CascadingParameter] private Task<AuthenticationState> AuthenticationState { get; set; }

        private List<Cluster> clusters;
        private PaginationDTO paginationDTO = new PaginationDTO() { RecordsPerPage = 3};

        private int totalAmountPages;

        protected override async Task OnInitializedAsync()
        {
            var authState = await AuthenticationState;
            var user = authState.User;
            if(user.Identity.IsAuthenticated)
            {
                Console.WriteLine("Authenticated");
            }
            else
            {
                Console.WriteLine("Not Authenticated");
            }

            try
            {
                await LoadClusters();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }           
            
        }

        private async Task LoadClusters()
        {
            var paginatedResponse = await clusterRepository.GetClusters(paginationDTO);
            clusters = paginatedResponse.Response;
            totalAmountPages = paginatedResponse.TotalAmountPages;
            Console.WriteLine(paginatedResponse.TotalAmountPages);
        }

        private async Task SelectedPage(int page)
        {
            paginationDTO.Page = page;
            await LoadClusters();
        }

        private async Task AddNewCluster()
        {
            var newCluster = new Cluster { Guid = Guid.NewGuid(), Name = clusters.Count.ToString(), Caption = clusters.Count.ToString() + "caption" };
            await clusterRepository.CreateCluster(newCluster);
            await LoadClusters();
        }

        private async Task DeleteCluster(int id)
        {
            await clusterRepository.DeleteCluster(id);
            await LoadClusters();
        }
    }
}
