#include <iostream>
#include <vector>
#include <string>

// ESTRUCTURA LISTA, SOLO FALTA CREAR LAS FUNCIONES
struct Inode {
    std::string nombre;
    std::string permisos = "rwx"; // Todos los permisos por defecto
    int identificador;
    int tamaño;
    bool es_directorio;
    Inode* padre;
    std::vector<Inode*> hijos;
};

void touch(Inode* directorio, const std::string& nombre_archivo, int identificador) {
    Inode* nuevo_archivo = new Inode();
    nuevo_archivo->nombre = nombre_archivo;
    nuevo_archivo->es_directorio = false;
    nuevo_archivo->identificador = identificador;
    nuevo_archivo->padre = directorio;
    directorio->hijos.push_back(nuevo_archivo);
}

void mkdir(Inode* directorio, const std::string& nombre_directorio, int identificador) {
    Inode* nuevo_directorio = new Inode();
    nuevo_directorio->nombre = nombre_directorio;
    nuevo_directorio->es_directorio = true;
    nuevo_directorio->identificador = identificador;
    nuevo_directorio->padre = directorio;
    directorio->hijos.push_back(nuevo_directorio);
}

void chmod(Inode* nodo, const std::string& permisos) {
    nodo->permisos = permisos;
}

void ls(Inode* nodo) {
    for(auto hijo : nodo->hijos) {
        std::cout << hijo->nombre << "\n";
    }
}

void ls_i(Inode* nodo) {
    for(auto hijo : nodo->hijos) {
        std::cout << hijo->nombre << " " << hijo->identificador << "\n";
    }
}

void ls_R(Inode* nodo) {
    ls(nodo);
    for(auto hijo : nodo->hijos) {
        if(hijo->es_directorio) {
            ls_R(hijo);
        }
    }
}

Inode* cd(Inode* nodo, const std::string& nombre_directorio) {
    for(auto hijo : nodo->hijos) {
        if(hijo->es_directorio && hijo->nombre == nombre_directorio) {
            return hijo;
        }
    }
    return nodo; // Si el directorio no se encuentra, se queda en el nodo actual
}

void rm(Inode* nodo, const std::string& nombre_archivo) {
    for(auto it = nodo->hijos.begin(); it != nodo->hijos.end(); ++it) {
        if(!(*it)->es_directorio && (*it)->nombre == nombre_archivo) {
            delete *it;
            nodo->hijos.erase(it);
            return;
        }
    }
}

void rm_r(Inode* nodo, const std::string& nombre_directorio) {
    for(auto it = nodo->hijos.begin(); it != nodo->hijos.end(); ++it) {
        if((*it)->es_directorio && (*it)->nombre == nombre_directorio) {
            for(auto hijo : (*it)->hijos) {
                if(hijo->es_directorio) {
                    rm_r(hijo, hijo->nombre);
                }
                else {
                    rm(*it, hijo->nombre);
                }
            }
            delete *it;
            nodo->hijos.erase(it);
            return;
        }
    }
}

void mv(Inode* nodo_origen, const std::string& nombre, Inode* nodo_destino) {
    for(auto it = nodo_origen->hijos.begin(); it != nodo_origen->hijos.end(); ++it) {
        if((*it)->nombre == nombre) {
            (*it)->padre = nodo_destino;
            nodo_destino->hijos.push_back(*it);
            nodo_origen->hijos.erase(it);
            return;
        }
    }
}

void print_tree(Inode* nodo, std::string prefix = "") {
    std::cout << prefix << nodo->nombre << "\n";
    prefix += "  ";
    for(auto hijo : nodo->hijos) {
        print_tree(hijo, prefix);
    }
}

int main() {
    // Crear el directorio raíz
    Inode* root = new Inode();
    root->nombre = "/";
    root->es_directorio = true;
    root->identificador = 0; // El identificador del directorio raíz es 0

    // Crear un nuevo directorio en la raíz
    mkdir(root, "MiDirectorio", 1); // El identificador del directorio es 1

    // Buscar el directorio que acabamos de crear
    Inode* miDirectorio = cd(root, "MiDirectorio");
    if (miDirectorio != root) {
        // Si encontramos el directorio, crear los archivos
        touch(miDirectorio, "Archivo1.txt", 2); // El identificador del archivo1 es 2
        touch(miDirectorio, "Archivo2.txt", 3); // El identificador del archivo2 es 3

        // Listar los archivos en el directorio
        // ls(miDirectorio);
    }

    print_tree(root);

    // No olvides liberar la memoria al final!
    rm_r(root, "MiDirectorio");
    delete root;

    return 0;
}







