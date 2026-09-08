#pragma once
#include <filesystem>
#include "Core/File.hpp"
#include "Menu/Folder.hpp"

namespace Stand
{
	class FileMgr final
	{
	private:
		FileMgr() = default;

	public:
		virtual ~FileMgr() = default;

		FileMgr(const FileMgr&) = delete;
		FileMgr(FileMgr&&) noexcept = delete;
		FileMgr& operator=(const FileMgr&) = delete;
		FileMgr& operator=(FileMgr&&) noexcept = delete;

		static void Init(const std::filesystem::path& rootFolder);

		[[nodiscard]] static File GetProjectFile(const std::filesystem::path& file)
		{
			return GetInstance().GetProjectFileImpl(file);
		}
		[[nodiscard]] static Folder GetProjectFolder(const std::filesystem::path& folder)
		{
			return GetInstance().GetProjectFolderImpl(folder);
		}

		static std::filesystem::path CreateFolderIfNotExists(const std::filesystem::path& folder);
		static std::filesystem::path EnsureFileCanBeCreated(const std::filesystem::path& file);

	private:
		void InitImpl(const std::filesystem::path& rootFolder);

		File GetProjectFileImpl(const std::filesystem::path& file) const;
		Folder GetProjectFolderImpl(const std::filesystem::path& folder) const;

		static FileMgr& GetInstance()
		{
			static FileMgr i{};

			return i;
		}

	private:
		std::filesystem::path m_RootFolder;
	};
}