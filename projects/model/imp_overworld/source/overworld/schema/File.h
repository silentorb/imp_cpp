#pragma once

#include <string>
#include <unordered_map>

namespace overworld {

  class File {
      std::string folder;
      std::string filename;
      bool _is_external = false;

  public:
      File() {}

      File(const std::string &folder, const std::string &filename) :
        folder(folder), filename(filename) {}

      File(const std::string &filename, bool is_external = false) :
        folder(""), filename(filename), _is_external(is_external) {}

      const std::string get_folder() const {
        return folder;
      }

      const std::string get_filename() const {
        return filename;
      }

      bool is_external() const {
        return _is_external;
      }
  };

//  class External_File : public File {
//  public:
//      External_File(const std::string &filename) : File(filename) {}
//
//      bool is_external() const override {
//        return true;
//      }
//  };

  class File_Reference {
      const File &file;
      bool _is_local;

  public:
      File_Reference(const File &file, bool is_local) : file(file), _is_local(is_local) {}

      const File &get_file() const {
        return file;
      }

      bool is_local() const {
        if (file.is_external())
          return false;

        return _is_local;
      }

      const std::string get_folder() const {
        return file.get_folder();
      }

      const std::string get_filename() const {
        return file.get_filename();
      }
  };

  using File_Library = std::unordered_map<std::string, File>;
}