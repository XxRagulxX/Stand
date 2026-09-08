#include "Core/FileMgr.hpp"
#include "Core/File.hpp"
#include "Menu/Folder.hpp"

namespace Stand
{
	void FileMgr::Init(const std::filesystem::path& rootFolder)
	{
		GetInstance().InitImpl(rootFolder);
	}

	void FileMgr::InitImpl(const std::filesystem::path& rootFolder)
	{
		m_RootFolder = rootFolder;

		CreateFolderIfNotExists(m_RootFolder);
	}

	std::filesystem::path FileMgr::CreateFolderIfNotExists(const std::filesystem::path& folder)
	{
		bool create_path = !exists(folder);

		if (!create_path && !is_directory(folder))
		{
			std::filesystem::remove(folder);
			create_path = true;
		}
		if (create_path)
			create_directories(folder);

		return folder;
	}

	std::filesystem::path FileMgr::EnsureFileCanBeCreated(const std::filesystem::path& file)
	{
		return FileMgr::CreateFolderIfNotExists(file.parent_path());
	}

	File FileMgr::GetProjectFileImpl(const std::filesystem::path& file) const
	{
		if (file.is_absolute())
			throw std::invalid_argument("FileMgr::GetProjectFile expects a relative path.");

		auto projFile = File(m_RootFolder / file);
		EnsureFileCanBeCreated(m_RootFolder / file);

		return projFile;
	}

	Folder FileMgr::GetProjectFolderImpl(const std::filesystem::path& folder) const
	{
		if (folder.is_absolute())
			throw std::invalid_argument("FileMgr::GetProjectFolder expects a relative path.");

		return CreateFolderIfNotExists(m_RootFolder / folder);
	}
}