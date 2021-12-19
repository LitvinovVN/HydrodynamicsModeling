#See https://aka.ms/containerfastmode to understand how Visual Studio uses this Dockerfile to build your images for faster debugging.

FROM mcr.microsoft.com/dotnet/aspnet:6.0 AS base
WORKDIR /app
EXPOSE 80
EXPOSE 443

FROM mcr.microsoft.com/dotnet/sdk:6.0 AS build
WORKDIR /src
COPY ["FrontendBlazor/Server/FrontendBlazor.Server.csproj", "FrontendBlazor/Server/"]
COPY ["FrontendBlazor/Shared/FrontendBlazor.Shared.csproj", "FrontendBlazor/Shared/"]
COPY ["FrontendBlazor/Client/FrontendBlazor.Client.csproj", "FrontendBlazor/Client/"]
RUN dotnet restore "FrontendBlazor/Server/FrontendBlazor.Server.csproj"
COPY . .
WORKDIR "/src/FrontendBlazor/Server"
RUN dotnet build "FrontendBlazor.Server.csproj" -c Release -o /app/build

FROM build AS publish
RUN dotnet publish "FrontendBlazor.Server.csproj" -c Release -o /app/publish

FROM base AS final
WORKDIR /app
COPY --from=publish /app/publish .
ENTRYPOINT ["dotnet", "FrontendBlazor.Server.dll"]