using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace QuickUsbFileUpdate
{
    public partial class mainForm : Form
    {
        List<string> FileSearch = new List<string>();

        public mainForm()
        {
            InitializeComponent();
        }

        private void mainForm_Load(object sender, EventArgs e)
        {
            LoadData();
        }

        private void mainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveData();
        }

        private void bBrowseSetupFileDir_Click(object sender, EventArgs e)
        {
            // Set the directory
            folderBrowserDialog.SelectedPath = tbSetupFileDir.Text;

            // Display the FileOpen dialog
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                tbSetupFileDir.Text = folderBrowserDialog.SelectedPath;
            }
        }

        private void bBrowseQuickUSBProjectDir_Click(object sender, EventArgs e)
        {
            // Set the directory
            folderBrowserDialog.SelectedPath = tbQuickUSBProjectDir.Text;

            // Display the FileOpen dialog
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                tbQuickUSBProjectDir.Text = folderBrowserDialog.SelectedPath;
            }
        }

        private void bAddSource_Click(object sender, EventArgs e)
        {
            // Set the directory
            openFileDialog.InitialDirectory = tbQuickUSBProjectDir.Text;
            openFileDialog.Filter = "All Files (*.*)|*.*";
            openFileDialog.Multiselect = true;

            // Display the FileOpen dialog
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                foreach (string fileName in openFileDialog.FileNames)
                {
                    AddSource(fileName);
                }
            }
        }

        TreeNode AddSource(string sourceFile)
        {
            // Check that the source does not already exist
            foreach (TreeNode treeNodeSource in tvFiles.Nodes)
            {
                FileNode fileNodeSource = (FileNode)treeNodeSource.Tag;
                if (fileNodeSource.Type == FileNode.NodeType.SourceFile)
                {
                    if (sourceFile == fileNodeSource.Path)
                    {
                        // The source already exists so just return it
                        return treeNodeSource;
                    }
                }
            }

            FileNode fileNode = new FileNode();
            fileNode.Type = FileNode.NodeType.SourceFile;
            fileNode.Path = sourceFile;
            fileNode.Enabled = true;

            TreeNode treeNode = new TreeNode(fileNode.Path.Replace(tbQuickUSBProjectDir.Text, ""));
            treeNode.Tag = fileNode;
            tvFiles.Nodes.Add(treeNode);

            treeNode.Expand();
            tvFiles.SelectedNode = treeNode;

            return treeNode;
        }

        private void bAddLink_Click(object sender, EventArgs e)
        {
            // Set the directory
            openFileDialog.InitialDirectory = tbSetupFileDir.Text;
            openFileDialog.Filter = "All Files (*.*)|*.*";
            openFileDialog.Multiselect = true;

            // Display the FileOpen dialog
            TreeNode sourceTreeNode = tvFiles.SelectedNode;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                foreach (string fileName in openFileDialog.FileNames)
                {
                    tvFiles.SelectedNode = AddLink(sourceTreeNode, fileName);
                }
            }
        }

        TreeNode AddLink(TreeNode sourceTreeNode, string linkFile)
        {
            FileNode sourceFileNode = (FileNode)sourceTreeNode.Tag;

            FileNode fileNode = new FileNode();
            fileNode.Type = FileNode.NodeType.LinkedFile;
            fileNode.Path = linkFile;
            fileNode.Enabled = true;

            TreeNode treeNode = new TreeNode(fileNode.Path.Replace(tbQuickUSBProjectDir.Text, ""));
            treeNode.Tag = fileNode;

            sourceTreeNode.Nodes.Add(treeNode);

            return treeNode;
        }

        private void tvFiles_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (tvFiles.SelectedNode != null)
            {
                FileNode fileNode = (FileNode)tvFiles.SelectedNode.Tag;
                bAddLink.Enabled = (fileNode.Type == FileNode.NodeType.SourceFile);
                bScanForLinks.Enabled = (fileNode.Type == FileNode.NodeType.SourceFile);
                bRemoveLink.Enabled = fileNode.Enabled && (fileNode.Type == FileNode.NodeType.LinkedFile);
                bRemoveSource.Enabled = fileNode.Enabled && (fileNode.Type == FileNode.NodeType.SourceFile);
            }
            else
            {
                bAddLink.Enabled = false;
                bScanForLinks.Enabled = false;
                bRemoveLink.Enabled = false;
                bRemoveSource.Enabled = false;
            }
        }

        private void bScanForLinks_Click(object sender, EventArgs e)
        {
            TreeNode treeNode = tvFiles.SelectedNode;
            FileNode fileNode = (FileNode)treeNode.Tag;

            // Search for files
            FileSearch.Clear();
            DirSearch(tbSetupFileDir.Text, Path.GetFileName(fileNode.Path));

            // Add links to the source file
            foreach (string link in FileSearch)
            {
                // Check that the link does not already exist
                bool alreadyExists = false;
                foreach (TreeNode treeNodeLink in treeNode.Nodes)
                {
                    FileNode fileNodeLink = (FileNode)treeNodeLink.Tag;
                    if (fileNodeLink.Type == FileNode.NodeType.LinkedFile)
                    {
                        if (link == fileNodeLink.Path)
                        {
                            alreadyExists = true;
                        }
                    }
                }

                if (!alreadyExists)
                {
                    AddLink(treeNode, link);
                }
            }
        }

        int CreateLinksByFileName(string sourceFile, string linkDirectory)
        {
            int linksCreated = 0;

            // Search for files that have a matching name in the link directory
            FileSearch.Clear();
            DirSearch(linkDirectory, Path.GetFileName(sourceFile));

            // Loop through all the files that will be linked by the source file
            foreach (string link in FileSearch)
            {
                // Add the source file
                TreeNode sourceTreeNode = AddSource(sourceFile);
                AddLink(sourceTreeNode, link);
                sourceTreeNode.Expand();
                ++linksCreated;
            }

            return linksCreated;
        }

        void LinkDirectories(string sourceDir, string linkDir)
        {
            // Get a list of all source files
            FileSearch.Clear();
            GetFiles(sourceDir);
            List<string> sourceFiles = new List<string>(FileSearch);

            // Loop through all source files and create links
            foreach (string sourceFile in sourceFiles)
            {
                int linksCreated = CreateLinksByFileName(sourceFile, linkDir);
            }
        }

        List<FileNode> GetAllLinks(bool onlyEnabledLinks)
        {
            List<FileNode> fileNodeList = new List<FileNode>();

            // Loop through all sources
            foreach (TreeNode treeNodeSource in tvFiles.Nodes)
            {
                FileNode fileNodeSource = (FileNode)treeNodeSource.Tag;
                if (fileNodeSource.Type == FileNode.NodeType.SourceFile)
                {
                    // Loop through all links
                    foreach (TreeNode treeNodeLink in treeNodeSource.Nodes)
                    {
                        FileNode fileNodeLink = (FileNode)treeNodeLink.Tag;
                        if (fileNodeLink.Type == FileNode.NodeType.LinkedFile)
                        {
                            if (!onlyEnabledLinks || fileNodeLink.Enabled)
                            {
                                fileNodeList.Add(fileNodeLink);
                            }
                        }
                    }
                }
            }

            return fileNodeList;
        }

        List<FileNode> GetAllSources(bool onlyEnabledSources)
        {
            List<FileNode> fileNodeList = new List<FileNode>();

            // Loop through all sources
            foreach (TreeNode treeNodeSource in tvFiles.Nodes)
            {
                FileNode fileNodeSource = (FileNode)treeNodeSource.Tag;
                if (fileNodeSource.Type == FileNode.NodeType.SourceFile)
                {
                    if (!onlyEnabledSources || fileNodeSource.Enabled)
                    {
                        fileNodeList.Add(fileNodeSource);
                    }
                }
            }

            return fileNodeList;
        }

        FileNode GetLinkSource(FileNode linkNode)
        {
            // Loop through all sources
            foreach (TreeNode treeNodeSource in tvFiles.Nodes)
            {
                FileNode fileNodeSource = (FileNode)treeNodeSource.Tag;
                if (fileNodeSource.Type == FileNode.NodeType.SourceFile)
                {
                    // Loop through all links
                    foreach (TreeNode treeNodeLink in treeNodeSource.Nodes)
                    {
                        FileNode fileNodeLink = (FileNode)treeNodeLink.Tag;
                        if (fileNodeLink == linkNode)
                        {
                            return fileNodeSource;
                        }
                    }
                }
            }

            return null;
        }

        private void bLinkDirectory_Click(object sender, EventArgs e)
        {
            string sourceDir, linkDir;

            // Set the directory
            folderBrowserDialog.SelectedPath = tbQuickUSBProjectDir.Text;
            folderBrowserDialog.Description = "Select a source directory";

            // Display the FileOpen dialog
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                sourceDir = folderBrowserDialog.SelectedPath;
            }
            else
            {
                return;
            }

            // Set the directory
            folderBrowserDialog.SelectedPath = tbSetupFileDir.Text;
            folderBrowserDialog.Description = "Select a directory to link to";

            // Display the FileOpen dialog
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                linkDir = folderBrowserDialog.SelectedPath;
            }
            else
            {
                return;
            }

            LinkDirectories(sourceDir, linkDir);
        }

        void DirSearch(string dir, string file)
        {
            foreach (string f in Directory.GetFiles(dir, file))
            {
                FileSearch.Add(f);
            }

            foreach (string d in Directory.GetDirectories(dir))
            {
                if (d.EndsWith(".svn"))
                {
                    continue;
                }
                DirSearch(d, file);
            }
        }

        void GetFiles(string dir)
        {
            foreach (string f in Directory.GetFiles(dir))
            {
                FileSearch.Add(f);
            }

            foreach (string d in Directory.GetDirectories(dir))
            {
                if ((d == dir) || d.EndsWith(".svn"))
                {
                    continue;
                }
                GetFiles(d);
            }
        }

        private void bRemoveLink_Click(object sender, EventArgs e)
        {
            TreeNode treeNode = tvFiles.SelectedNode;
            FileNode fileNode = (FileNode)treeNode.Tag;
            treeNode.NodeFont = new Font(tvFiles.Font, FontStyle.Strikeout);
            fileNode.Enabled = false;
        }

        private void bRemoveSource_Click(object sender, EventArgs e)
        {
            TreeNode treeNode = tvFiles.SelectedNode;
            tvFiles.Nodes.Remove(treeNode);
        }

        private void bPerformFileUpdate_Click(object sender, EventArgs e)
        {
            tbLog.Clear();

            int count = 0;

            // Loop through all sources
            foreach (TreeNode treeNodeSource in tvFiles.Nodes)
            {
                FileNode fileNodeSource = (FileNode)treeNodeSource.Tag;
                if (fileNodeSource.Type == FileNode.NodeType.SourceFile)
                {
                    // Loop through all links
                    foreach (TreeNode treeNodeLink in treeNodeSource.Nodes)
                    {
                        FileNode fileNodeLink = (FileNode)treeNodeLink.Tag;
                        if ((fileNodeLink.Type == FileNode.NodeType.LinkedFile) && fileNodeLink.Enabled)
                        {
                            // Perform the file update
                            tbLog.AppendText(String.Format("[{0}] Updating <SetupDir>{1} with <ProjectDir>{2}" + Environment.NewLine, count, fileNodeLink.Path.Replace(tbSetupFileDir.Text, ""), fileNodeSource.Path.Replace(tbQuickUSBProjectDir.Text, "")));
                            File.Copy(fileNodeSource.Path, fileNodeLink.Path, true);
                            ++count;

                            Application.DoEvents();
                        }
                    }
                }
            }
            tbLog.AppendText("Done.");
        }

        private void SaveData()
        {
            using (StreamWriter file = new StreamWriter(@"data.txt", false))
            {
                // Loop through all sources
                foreach (TreeNode treeNodeSource in tvFiles.Nodes)
                {
                    FileNode fileNodeSource = (FileNode)treeNodeSource.Tag;
                    if (fileNodeSource.Type == FileNode.NodeType.SourceFile)
                    {
                        // Write source to file
                        string sourceEnabled = (fileNodeSource.Enabled) ? "Enabled" : "Disabled";
                        file.WriteLine("Source " + sourceEnabled + " " + fileNodeSource.Path);

                        // Loop through all links
                        foreach (TreeNode treeNodeLink in treeNodeSource.Nodes)
                        {
                            FileNode fileNodeLink = (FileNode)treeNodeLink.Tag;
                            if (fileNodeLink.Type == FileNode.NodeType.LinkedFile)
                            {
                                // Write link to file
                                string linkEnabled = (fileNodeLink.Enabled) ? "Enabled" : "Disabled";
                                file.WriteLine("\tLink " + linkEnabled + " " + fileNodeLink.Path);
                            }
                        }
                    }
                }
            }
        }

        private void LoadData()
        {
            if (!File.Exists(@"data.txt"))
            {
                return;
            }

            using (StreamReader file = new StreamReader(@"data.txt"))
            {
                TreeNode parentSourceNode = null;

                do
                {
                    string line = file.ReadLine();
                    if (line == null)
                    {
                        break;
                    }

                    line = line.Trim();

                    if (line == "")
                    {
                        continue;
                    }

                    string[] tokens = line.Split(' ');
                    if (tokens.Length < 3)
                    {
                        continue;
                    }
                    line = line.Remove(0, tokens[0].Length).Trim();
                    line = line.Remove(0, tokens[1].Length).Trim();

                    if (tokens[0] == "Source")
                    {
                        // Add the source to the tree
                        line.Remove(0, 7);

                        FileNode newFileNode = new FileNode();
                        newFileNode.Type = FileNode.NodeType.SourceFile;
                        newFileNode.Path = line;

                        TreeNode newTreeNode = new TreeNode(line.Replace(tbQuickUSBProjectDir.Text, ""));
                        newTreeNode.Tag = newFileNode;

                        if (tokens[1] == "Enabled")
                        {
                            newFileNode.Enabled = true;
                        }
                        else if (tokens[1] == "Disabled")
                        {
                            newFileNode.Enabled = false;
                            newTreeNode.NodeFont = new Font(tvFiles.Font, FontStyle.Strikeout);
                        }

                        tvFiles.Nodes.Add(newTreeNode);
                        parentSourceNode = newTreeNode;
                    }
                    else if (tokens[0]  == "Link")
                    {
                        // Add the link to the tree
                        line.Remove(0, 5);

                        FileNode newFileNode = new FileNode();
                        newFileNode.Type = FileNode.NodeType.LinkedFile;
                        newFileNode.Path = line;

                        TreeNode newTreeNode = new TreeNode(line.Replace(tbSetupFileDir.Text, ""));
                        newTreeNode.Tag = newFileNode;

                        if (tokens[1] == "Enabled")
                        {
                            newFileNode.Enabled = true;
                        }
                        else if (tokens[1] == "Disabled")
                        {
                            newFileNode.Enabled = false;
                            newTreeNode.NodeFont = new Font(tvFiles.Font, FontStyle.Strikeout);
                        }

                        parentSourceNode.Nodes.Add(newTreeNode);
                    }
                } while (true);
            }

            //tvFiles.ExpandAll();
            if (tvFiles.Nodes.Count > 0)
            {
                tvFiles.SelectedNode = tvFiles.Nodes[0];
            }
        }

        private void bSave_Click(object sender, EventArgs e)
        {
            SaveData();
        }

        private void bCheckForErrors_Click(object sender, EventArgs e)
        {
            tbLog.Clear();

            // Get a list of all files in the Setup directory
            FileSearch.Clear();
            GetFiles(tbSetupFileDir.Text);

            // Get a list of all the links we have
            List<FileNode> fileLinks = GetAllLinks(true);

            // Loop over each setup file
            int missingLinkCount = 0, multipleLinkCount = 0, missingSourceFileCount = 0, missingLinkFileCount = 0;
            foreach (string setupFile in FileSearch)
            {
                // Search through all of our links
                int linkCount = 0;
                foreach (FileNode fileNode in fileLinks)
                {
                    if (setupFile == fileNode.Path)
                    {
                        ++linkCount;
                    }
                }

                // Report if a setup file does not have a link
                if (linkCount == 0)
                {
                    ++missingLinkCount;
                    tbLog.AppendText(String.Format("Setup file <SetupDir>{0} has no links" + Environment.NewLine, setupFile.Replace(tbSetupFileDir.Text, "")));
                }

                // Report if a setup file has multiple links
                if (linkCount > 1)
                {
                    ++multipleLinkCount;
                    tbLog.AppendText(String.Format("Setup file <SetupDir>{0} has multiple links ({1})" + Environment.NewLine, setupFile.Replace(tbSetupFileDir.Text, ""), linkCount));

                    // Display all links
                    foreach (FileNode fileNode in fileLinks)
                    {
                        if (setupFile == fileNode.Path)
                        {
                            tbLog.AppendText(String.Format("--> <ProjectDir>{0}" + Environment.NewLine, GetLinkSource(fileNode).Path.Replace(tbQuickUSBProjectDir.Text, "")));
                        }
                    }
                }
            }

            // Loop through all sources
            foreach (TreeNode treeNodeSource in tvFiles.Nodes)
            {
                FileNode fileNodeSource = (FileNode)treeNodeSource.Tag;
                if (fileNodeSource.Type == FileNode.NodeType.SourceFile)
                {
                    // Verify that the source file exists
                    if (!File.Exists(fileNodeSource.Path))
                    {
                        tbLog.AppendText("Source file does not exists" + Environment.NewLine);
                        tbLog.AppendText("--> Source: <ProjectDir>" + fileNodeSource.Path + Environment.NewLine);
                        ++missingSourceFileCount;
                    }

                    // Loop through all links
                    foreach (TreeNode treeNodeLink in treeNodeSource.Nodes)
                    {
                        FileNode fileNodeLink = (FileNode)treeNodeLink.Tag;
                        if (fileNodeLink.Type == FileNode.NodeType.LinkedFile)
                        {
                            // Verify the link file exists
                            if (!File.Exists(fileNodeLink.Path))
                            {
                                tbLog.AppendText("Source points to link that does not exists" + Environment.NewLine);
                                tbLog.AppendText("--> Source: <ProjectDir>" + fileNodeSource.Path.Replace(tbQuickUSBProjectDir.Text, "") + Environment.NewLine);
                                tbLog.AppendText("--> Link:   <SetupDir>" + fileNodeLink.Path.Replace(tbSetupFileDir.Text, "") + Environment.NewLine);
                                ++missingLinkFileCount;
                            }
                        }
                    }
                }
            }

            // Report results
            tbLog.AppendText(Environment.NewLine);
            tbLog.AppendText("Missing Link Count: " + missingLinkCount + Environment.NewLine);
            tbLog.AppendText("Multiple Link Count: " + multipleLinkCount + Environment.NewLine);
            tbLog.AppendText("Missing Source File Count: " + missingSourceFileCount + Environment.NewLine);
            tbLog.AppendText("Missing Link File Count: " + missingLinkFileCount + Environment.NewLine);
        }
    }

    public class FileNode
    {
        public enum NodeType { SourceFile, LinkedFile };

        public NodeType Type;
        public string Path;
        public bool Enabled;
    }
}
