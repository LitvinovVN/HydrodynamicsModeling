#See https://aka.ms/containerfastmode to understand how Visual Studio uses this Dockerfile to build your images for faster debugging.

FROM mcr.microsoft.com/dotnet/aspnet:6.0 AS base
WORKDIR /app
EXPOSE 80

FROM mcr.microsoft.com/dotnet/sdk:6.0 AS build
WORKDIR /src
COPY ["ModelingSystem/Server/ModelingSystem.Server.csproj", "ModelingSystem/Server/"]
COPY ["ModelingSystem/Shared/ModelingSystem.Shared.csproj", "ModelingSystem/Shared/"]
COPY ["ModelingSystem/Client/ModelingSystem.Client.csproj", "ModelingSystem/Client/"]
RUN dotnet restore "ModelingSystem/Server/ModelingSystem.Server.csproj"
COPY . .
WORKDIR "/src/ModelingSystem/Server"
RUN dotnet build "ModelingSystem.Server.csproj" -c Release -o /app/build

FROM build AS publish
RUN dotnet publish "ModelingSystem.Server.csproj" -c Release -o /app/publish

FROM base AS final
WORKDIR /app
COPY --from=publish /app/publish .
ENTRYPOINT ["dotnet", "ModelingSystem.Server.dll"]
