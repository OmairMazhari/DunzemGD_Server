# Running the Project

To run the project on any platform you'll need to compile the GDExtension binaries for that platform which is should be placed the bin folder of the project.  
Since the source code for GDExtension of the project is not contained in the repo, the binaries for the windows and the web version will be precompiled in the repo.  
This does mean that the project will run natively only on windows as of now.

---

## Windows

### Running the project
- The project can be run from the editor as the binaries for windows are precompiled.

### Exporting
- The project can be exported from the editor, by going under  
  **Project → Export... → Add... → Windows Desktop → Export Project...**  
- Make sure **Export with Debug** is **unchecked** under the Export Project... part.  

---

## MacOS

### Running the project
- The project currently cannot be run on macOS. See **Exporting to Web** instead.

### Exporting to Web
- The project can be exported to web from the editor, by going under  
  **Project → Export... → Add... → Web → Export Project...**  
- Make sure **Export with Debug** is **unchecked** under the Export Project... part.  
- Make sure to **export the web project outside the repo**, Godot will then generate the relevant files to run the project on the web.  
- However, you'll first need to **host a local server** before running the project, since you can’t just open the HTML file directly due to browser security restrictions.  
- Install Python and open your terminal.  
- `cd` into the directory where you have the web export for the project.  
- Start the server in that directory with:  
  ```sh
  python -m http.server 8000
- Then open a browser and go to:  
  ```sh
  http://localhost:8000/[ProjectName].html
- Replace [ProjectName] with the name of your exported HTML file.

  
  
