#include "vfs.h"

#include <engine/assert.h>
#include <engine/settings.h>
#include <engine/utils/fs.h>

#include <EASTL/vector_map.h>

#include <memory>

namespace
{
  class SourceController
  {
    public:
      void mount(const string_view path, const string_view to)
      {
        m_Mounts.insert({path, string{to}});
      }

      eastl::vector<char> readFile(const string_view mount, const string_view path)
      {
        const string_view relPath = path.substr(mount.length());

        const auto it = m_Mounts.find(mount);
        ASSERT_FMT(it != m_Mounts.end(), "vfs: source mount {} does not exist", path);

        const string& mountDst = it->second;

        const size_t resolvedPathSize = mountDst.length() + relPath.length() + 1;
        char* resolvedPath = (char*)alloca(resolvedPathSize);

        std::memcpy(resolvedPath, mountDst.data(), mountDst.size());
        std::memcpy(resolvedPath + mountDst.size(), relPath.data(), relPath.length());
        resolvedPath[resolvedPathSize-1] = '\0';

        return Utils::read_file(resolvedPath);
      }
    private:
      eastl::vector_map<string_view, string> m_Mounts;
  };

  class VFS
  {
    public:
      void mountSource(const string_view mount_point, const string_view to)
      {
        ASSERT_FMT_RETURN(mount_point[0] == '@', ,
          "vfs: can't mount {} to {}: mount point has to start with `@`", mount_point, to);
        ASSERT_FMT_RETURN(mount_point.find_first_of("/\\") == std::string::npos, ,
          "vfs: can't mount {} to {}: mount point must not have / or \\", mount_point, to);
        ASSERT_FMT_RETURN(to.find_first_of("\\") == std::string::npos, ,
          "vfs: can't mount {} to {}: dst path must not have \\", mount_point, to);

        const auto it = m_MountTypes.find(mount_point);
        ASSERT_FMT_RETURN(it == m_MountTypes.end(), ,
          "vfs: can't mount source {} to {}: it's already mounted", mount_point, to);
        
        m_MountTypes.insert({mount_point, MountType::Source});
        m_SourceController.mount(mount_point, to);

        loginfo("vfs: mounted {} to {}", mount_point, to);
      }

      eastl::vector<char> readFile(const string_view path)
      {
        if (path[0] == '@')
        {
          ASSERT_FMT_RETURN(path.find_first_of("/\\") != std::string::npos, {},
            "vfs: can't readFile {}: there is only mount point without a file", path);

          const auto delPos = path.find_first_of("/");
          const string_view mount = path.substr(0, delPos);

          const auto it = m_MountTypes.find(mount);
          if (it != m_MountTypes.end())
          {
            if (it->second == MountType::Source)
              return m_SourceController.readFile(mount, path);
            
            ASSERT_FMT_RETURN(false, {}, "vfs: can't read file {}: it has unsupported mounting {}", path, (int)it->second);
          }

          ASSERT(!"vfs: path {} doesn't have a mount point");
        }

        return Utils::read_file(path);
      }

    private:
      enum class MountType
      {
        Source
        //PAK
      };
      
      SourceController m_SourceController;
      eastl::vector_map<string_view, MountType> m_MountTypes;
  };

  VFS vfsManager;
}

namespace vfs
{
  void init()
  {
    const ed::Scope& vfsEd = Engine::get_app_settings().getScope("vfs");
    for (const ed::Scope& ed : vfsEd.getScopes())
    {
      const string_view& mountPoint = ed.getName();
      const string_view dst = ed.getVariable<string_view>("source");

      ASSERT_FMT(dst != string_view{}, "vfs: source mount point {} doesn't have a dst", mountPoint);

      vfsManager.mountSource(mountPoint, dst);
    }
  }

  void mount_source(const string_view path, const string_view to)
  {
    vfsManager.mountSource(path,to);
  }

  eastl::vector<char> read_file(const string_view path)
  {
    return vfsManager.readFile(path);
  }
}