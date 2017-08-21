#pragma once

#include <string>

namespace overworld {

  class File {
      const std::string folder;
      const std::string filename;

  public:
      File(const std::string &folder, const std::string &filename) :
        folder(folder), filename(filename) {}

      File(const std::string &filename) :
        folder(""), filename(filename) {}

      const std::string get_folder() const {
        return folder;
      }

      const std::string get_filename() const {
        return filename;
      }

      virtual bool is_external() const {
        return false;
      }
  };

  class External_File : public File {
  public:
      External_File(const std::string &filename) : File(filename) {}

      bool is_external() const override {
        return true;
      }
  };

  class File_Reference {
      File &file;
      bool _is_local;

  public:
      File_Reference(File &file, bool is_local) : file(file), _is_local(is_local) {}

      File &get_file() const {
        return file;
      }

      bool is_local() const {
        return _is_local;
      }

      const std::string get_folder() const {
        return file.get_folder();
      }

      const std::string get_filename() const {
        return file.get_filename();
      }
  };
}