using Microsoft.Extensions.DependencyInjection;

namespace Piece.ProjectManagement;

public static class ProjectManagementServiceCollectionExtensions
{
    public static IServiceCollection AddProjectManagement(this IServiceCollection services)
    {
        services.AddTransient<IProjectManager, ProjectManager>();
        services.AddTransient<IProjectAssetService, ProjectAssetService>();
        services.AddTransient<IProjectSceneService, ProjectSceneService>();
        services.AddTransient<IProjectBuildService, ProjectBuildService>();

        // Register framework abstraction implementations if they were in this assembly (unlikely)
        // For now, this assembly just defines the interfaces and concrete ProjectManagement services.
        // The actual implementations of IAssetManager and ISceneManager will come from Piece.Framework.

        return services;
    }
}
