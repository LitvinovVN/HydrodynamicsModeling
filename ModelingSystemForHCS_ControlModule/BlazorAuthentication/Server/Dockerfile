#See https://aka.ms/containerfastmode to understand how Visual Studio uses this Dockerfile to build your images for faster debugging.

FROM mcr.microsoft.com/dotnet/aspnet:6.0 AS base
WORKDIR /app
EXPOSE 80
EXPOSE 443

FROM mcr.microsoft.com/dotnet/sdk:6.0 AS build
WORKDIR /src
COPY ["BlazorAuthentication/Server/BlazorAuthentication.Server.csproj", "BlazorAuthentication/Server/"]
COPY ["BlazorAuthentication/Shared/BlazorAuthentication.Shared.csproj", "BlazorAuthentication/Shared/"]
COPY ["BlazorAuthentication/Client/BlazorAuthentication.Client.csproj", "BlazorAuthentication/Client/"]
RUN dotnet restore "BlazorAuthentication/Server/BlazorAuthentication.Server.csproj"
COPY . .
WORKDIR "/src/BlazorAuthentication/Server"
RUN dotnet build "BlazorAuthentication.Server.csproj" -c Release -o /app/build

FROM build AS publish
RUN dotnet publish "BlazorAuthentication.Server.csproj" -c Release -o /app/publish

FROM base AS final
WORKDIR /app
COPY --from=publish /app/publish .
ENTRYPOINT ["dotnet", "BlazorAuthentication.Server.dll"]