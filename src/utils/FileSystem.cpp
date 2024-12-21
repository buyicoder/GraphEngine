#include <utils/FileSystem.h>


void CreateDirectory(const std::string& path)
{
    try
    {
        // ����ļ����Ƿ��Ѵ���
        if (!fs::exists(path))
        {
            // ����Ŀ¼
            if (fs::create_directory(path))
            {
                std::cout << "Directory created successfully: " << path << std::endl;
            }
            else
            {
                std::cout << "Failed to create directory: " << path << std::endl;
            }
        }
        else
        {
            std::cout << "Directory already exists: " << path << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
void CreateFileInDirectory(const std::string& filepath)
{
    // ��ȡĿ¼·��
    std::filesystem::path dir = std::filesystem::path(filepath).parent_path();

    // ���Ŀ¼�Ƿ���ڣ�����������򴴽�
    if (!std::filesystem::exists(dir))
    {
        std::filesystem::create_directories(dir);
        std::cout << "Directory created: " << dir << std::endl;
    }

    // �����ļ�
    std::ofstream file(filepath);
    if (file.is_open())
    {
        file << "File created inside a new directory!" << std::endl;
        file.close();
        std::cout << "File created successfully: " << filepath << std::endl;
    }
    else
    {
        std::cerr << "Failed to create the file: " << filepath << std::endl;
    }
}
void SaveToFile(const std::string& filePath, const std::string& content)
{
    std::ofstream file(filePath);
    if (file.is_open())
    {
        file << content; // д������
        file.close();
        std::cout << "File saved successfully: " << filePath << std::endl;
    }
    else
    {
        std::cerr << "Failed to save file: " << filePath << std::endl;
    }
}
void InitFiles() {
    CreateDirectory("./Windows");
}