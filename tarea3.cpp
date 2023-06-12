#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm>
#include <sstream>
#include <ncurses.h>

using namespace std;

// ESTRUCTURA LISTA, SOLO FALTA CREAR LAS FUNCIONES
struct Inode
{
    string nombre;
    string permisos = "rwx"; // Todos los permisos por defecto
    int identificador;
    int tamaño;
    bool es_directorio;
    Inode *padre;
    vector<Inode *> hijos;
};
void printCurrentDirectory(Inode *nodo, vector<string> rutaParts = vector<string>());

void touch(Inode *directorio, const string &nombre_archivo, int identificador)
{

    // Agregar el archivo al sistema de archivos en memoria
    Inode *nuevo_archivo = new Inode();
    nuevo_archivo->nombre = nombre_archivo;
    nuevo_archivo->es_directorio = false;
    nuevo_archivo->identificador = identificador;
    nuevo_archivo->padre = directorio;
    directorio->hijos.push_back(nuevo_archivo);
}

void mkdir(Inode *directorio, const string &nombre_directorio, int identificador)
{

    Inode *nuevo_directorio = new Inode();
    nuevo_directorio->nombre = nombre_directorio;
    nuevo_directorio->es_directorio = true;
    nuevo_directorio->identificador = identificador;
    nuevo_directorio->padre = directorio;
    directorio->hijos.push_back(nuevo_directorio);
}


void ls(Inode *nodo)
{

    for (auto hijo : nodo->hijos)
    {
        if (hijo->es_directorio)
        {
            // Imprime los directorios en azul
            cout << "\033[1;34m" << hijo->nombre << "\033[0m"
                 << "\n";
        }
        else
        {
            // Imprime los archivos sin color
            cout << hijo->nombre << "\n";
        }
    }
}

void ls_i(Inode *nodo)
{
    for (auto hijo : nodo->hijos)
    {
        if (hijo->es_directorio)
        {
            // Imprime los directorios en azul
            cout << "\033[1;34m" << hijo->nombre << "\033[0m"
                 << " " << hijo->identificador << "\n";
        }
        else
        {
            // Imprime los archivos sin color
            cout << hijo->nombre << " " << hijo->identificador << "\n";
        }
    }
}

void ls_R(Inode *nodo, string ruta = "/", string prefix = "")
{

    cout << prefix << ruta << ":" << endl;

    for (auto hijo : nodo->hijos)
    {
        if (hijo->es_directorio)
        {
            // Imprime los directorios en azul
            cout << prefix << "\033[1;34m" << hijo->nombre << "\033[0m"
                 << "\n";
        }
        else
        {
            // Imprime los archivos sin color
            cout << prefix << hijo->nombre << "\n";
        }
    }

    cout << endl;

    for (auto hijo : nodo->hijos)
    {
        if (hijo->es_directorio)
        {
            // Extiende la ruta y el prefijo para la recursión
            ls_R(hijo, ruta + hijo->nombre + "/", prefix + "  ");
        }
    }
}

Inode *cd(Inode *nodo, const string &nombre_directorio)
{
    // cout<<"cd "<<nombre_directorio<<"\n";

    for (auto hijo : nodo->hijos)
    {
        if (hijo->es_directorio && hijo->nombre == nombre_directorio)
        {
            return hijo;
        }
    }
    return nodo; // Si el directorio no se encuentra, se queda en el nodo actual
}

void rm(Inode *nodo, const string &nombre_archivo)
{

    for (auto it = nodo->hijos.begin(); it != nodo->hijos.end(); ++it)
    {
        if (!(*it)->es_directorio && (*it)->nombre == nombre_archivo)
        {
            delete *it;
            nodo->hijos.erase(it);
            return;
        }
    }
}

void rm_r(Inode *nodo, const string &nombre_directorio)
{

    for (auto it = nodo->hijos.begin(); it != nodo->hijos.end(); ++it)
    {
        if ((*it)->es_directorio && (*it)->nombre == nombre_directorio)
        {
            for (auto hijo : (*it)->hijos)
            {
                if (hijo->es_directorio)
                {
                    rm_r(hijo, hijo->nombre);
                }
                else
                {
                    rm(*it, hijo->nombre);
                }
            }
            delete *it;
            nodo->hijos.erase(it);
            return;
        }
    }
}

void mv(Inode *nodo_origen, const string &nombre, Inode *nodo_destino)
{

    for (auto it = nodo_origen->hijos.begin(); it != nodo_origen->hijos.end(); ++it)
    {
        if ((*it)->nombre == nombre)
        {
            (*it)->padre = nodo_destino;
            nodo_destino->hijos.push_back(*it);
            nodo_origen->hijos.erase(it);
            return;
        }
    }
}


void print_tree(Inode *nodo, string prefix = "", bool isLast = true)
{
    cout << prefix;

    // Seleccionar el carácter del prefijo en función de si este es el último hijo o no
    string childPrefix = isLast ? "└──" : "├──";

    // Imprimir el nombre del nodo con un color diferente si es un directorio
    if (nodo->es_directorio)
    {
        cout << childPrefix << "\033[1;34m" << nodo->nombre << "\033[0m\n"; // Azul para directorios
    }
    else
    {
        cout << childPrefix << nodo->nombre << "\n"; // Sin color para archivos
    }

    // Crear el nuevo prefijo que se pasará a los hijos
    prefix += isLast ? "    " : "│   ";

    // Llamar a print_tree de forma recursiva para los hijos
    int numChildren = nodo->hijos.size();
    for (int i = 0; i < numChildren; ++i)
    {
        print_tree(nodo->hijos[i], prefix, i == numChildren - 1);
    }
}


void pwd(Inode *nodo, string path = "")
{
    // Si el nodo no es la raíz, añadimos su nombre al camino
    if (nodo->padre != nullptr)
    {
        path = nodo->nombre + (path.empty() ? "" : "/") + path;
        pwd(nodo->padre, path); // Llamada recursiva para seguir construyendo el camino
    }
    else
    {
        // Estamos en la raíz, entonces imprimimos el camino completo
        cout<<'/'<<path<<endl;
    }
}


void cd_back(Inode *&currentDirectory)
{
    cout << "\033[35m\nComando cd ..\033[0m" << endl
         << endl;

    if (currentDirectory->padre != nullptr)
    {
        currentDirectory = currentDirectory->padre;
    }
    else
    {
        cout << "Estas en el directorio raíz, no puedes retroceder más.\n"
             << endl;
    }
    this_thread::sleep_for(chrono::seconds(2));
}

void printCurrentDirectory(Inode *nodo, vector<string> rutaParts)
{
    rutaParts.push_back(nodo->nombre);
    if (nodo->padre != nullptr)
    {
        printCurrentDirectory(nodo->padre, rutaParts);
    }
    else
    {
        reverse(rutaParts.begin(), rutaParts.end());
        string ruta = "\033[92mpipe@pipe~:";
        for (const auto &part : rutaParts)
        {
            if (part != "/")
            {
                ruta += "\033[34m/" + part + "\033[0m";
            }
        }
        ruta += "\033[97m$ "; // Cambia a blanco el signo de $
        cout << ruta;
    }
}

vector<string> split(const string &str, char delim)
{
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim))
    {
        tokens.push_back(token);
    }
    return tokens;
}

int main()
{
    string command;
    vector<string> cmd_parts;
    int nextId = 1;

    Inode *root = new Inode();
    root->es_directorio = true;
    root->identificador = 0;
    root->nombre = "/";

    Inode *currentDirectory = root;

    while (true)
    {
        printCurrentDirectory(currentDirectory);
        getline(cin, command);
        cmd_parts = split(command, ' ');

        if (cmd_parts.size() == 0)
        {
            continue;
        }
        if (cmd_parts[0] == "exit")
        {
            break;
        }
        if (cmd_parts[0] == "ls")
        {
            if (cmd_parts.size() > 1 && cmd_parts[1] == "-i")
            {
                ls_i(currentDirectory);
            }
            else if (cmd_parts.size() > 1 && (cmd_parts[1] == "-R" || cmd_parts[1] == "-r"))
            {
                ls_R(currentDirectory);
            }
            else
            {
                ls(currentDirectory);
            }
        }
        else if (cmd_parts[0] == "cd")
        {
            if (cmd_parts.size() < 2)
            {
                cout << "Nombre de directorio requerido para cd" << endl;
            }
            else
            {
                if (cmd_parts[1] == "..")
                {
                    if (currentDirectory->padre != nullptr)
                    {
                        currentDirectory = currentDirectory->padre;
                    }
                }
                else
                {
                    currentDirectory = cd(currentDirectory, cmd_parts[1]);
                }
            }
        }
        else if (cmd_parts[0] == "touch")
        {
            if (cmd_parts.size() < 2)
            {
                cout << "Nombre de archivo requerido para touch" << endl;
            }
            else
            {
                touch(currentDirectory, cmd_parts[1], nextId++);
            }
        }
        else if (cmd_parts[0] == "mkdir")
        {
            if (cmd_parts.size() < 2)
            {
                cout << "Nombre de directorio requerido para mkdir" << endl;
            }
            else
            {
                mkdir(currentDirectory, cmd_parts[1], nextId++);
            }
        }
        else if (cmd_parts[0] == "rm")
        {
            if (cmd_parts.size() < 2)
            {
                cout << "Nombre del directorio requerido para rm" << endl;
            }
            else
            {
                if (cmd_parts.size() > 2 && cmd_parts[1] == "-r")
                {
                    rm_r(currentDirectory, cmd_parts[2]);
                }
                else
                {
                    rm(currentDirectory, cmd_parts[1]);
                }
            }
        }
        else if (cmd_parts[0] == "mv")
        {
            if (cmd_parts.size() < 3)
            {
                cout << "Se requieren dos argumentos para mv: nombre del archivo o directorio y nombre del directorio destino" << endl;
            }
            else
            {
                Inode *dest = cd(currentDirectory, cmd_parts[2]);
                if (dest != currentDirectory)
                {
                    mv(currentDirectory, cmd_parts[1], dest);
                }
                else
                {
                    cout << "El directorio destino no existe" << endl;
                }
            }
        }
        else if (cmd_parts[0] == "pwd")
        {
            pwd(currentDirectory);
        }
        else if (cmd_parts[0] == "clear")
        {
            #ifdef _WIN32
                system("CLS");
            #else
                system("clear");
            #endif
        }
        else if (cmd_parts[0] == "tree"){
            print_tree(root);
        }
        else
        {
            cout << "Comando no reconocido: " << cmd_parts[0] << endl;
        }
    }

    delete root;
    return 0;
}
