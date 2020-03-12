
//Title:       QuickUSB Diagnosics For Java
//Version:     1.0
//Copyright:   Copyright (c) B. Henry
//Author:      B. Henry
//Company:     Bitwise Systems
//Description:

import java.awt.*;
import javax.swing.*;
import QuickUsbBean.*;

public class QuickUsbDiagApplet extends JApplet {
   boolean isStandalone = false;
   JTabbedPane jTabbedPane1 = new JTabbedPane();
   JPanel jPanel1 = new JPanel();
   JPanel jPanel2 = new JPanel();
   JMenuBar jMenuBar1 = new JMenuBar();
   JMenu jMenu1 = new JMenu();
   JMenuItem jMenuItem1 = new JMenuItem();
  JTextField StatusBar = new JTextField();
  QuickUsbBean quickUsbBean1 = new QuickUsbBean();

   //Get a parameter value
   public String getParameter(String key, String def) {
      return isStandalone ? System.getProperty(key, def) :
         (getParameter(key) != null ? getParameter(key) : def);
   }

   //Construct the applet
   public QuickUsbDiagApplet() {
   }

   //Initialize the applet
   public void init() {
      try {
         jbInit();
      }
      catch(Exception e) {
         e.printStackTrace();
      }
   }

   //Component initialization
   private void jbInit() throws Exception {
      this.setJMenuBar(jMenuBar1);
    this.setSize(new Dimension(508, 438));
      jMenu1.setText("File");
      jMenuItem1.setText("Exit");
    StatusBar.setEditable(false);
    StatusBar.setText("");
    quickUsbBean1.setBorder(BorderFactory.createLineBorder(Color.black));
    jTabbedPane1.add(jPanel1,   "jPanel1");
    jTabbedPane1.add(jPanel2,  "jPanel2");
    this.getContentPane().add(quickUsbBean1,  BorderLayout.NORTH);
    this.getContentPane().add(StatusBar,  BorderLayout.SOUTH);
    this.getContentPane().add(jTabbedPane1, BorderLayout.CENTER);
      jMenuBar1.add(jMenu1);
      jMenu1.add(jMenuItem1);
   }

   //Start the applet
   public void start() {
   }

   //Stop the applet
   public void stop() {
   }

   //Destroy the applet
   public void destroy() {
   }

   //Get Applet information
   public String getAppletInfo() {
      return "Applet Information";
   }

   //Get parameter info
   public String[][] getParameterInfo() {
      return null;
   }

   //Main method
   public static void main(String[] args) {
      QuickUsbDiagApplet applet = new QuickUsbDiagApplet();
      applet.isStandalone = true;
      JFrame frame = new JFrame();
      //EXIT_ON_CLOSE == 3
      frame.setDefaultCloseOperation(3);
      frame.setTitle("Applet Frame");
      frame.getContentPane().add(applet, BorderLayout.CENTER);
      applet.init();
      applet.start();
      frame.setSize(400,320);
      Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
      frame.setLocation((d.width - frame.getSize().width) / 2, (d.height - frame.getSize().height) / 2);
      frame.setVisible(true);
   }

   //static initializer for setting look & feel
   static {
      try {
         //UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
         //UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
      }
      catch(Exception e) {
      }
   }
}
