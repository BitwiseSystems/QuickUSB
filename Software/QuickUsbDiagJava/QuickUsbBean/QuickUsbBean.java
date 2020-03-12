package QuickUsbBean;

import java.awt.*;
import javax.swing.*;

/**
 * <p>Title: QuickUSB Diagnosics For Java</p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) B. Henry</p>
 * <p>Company: Bitwise Systems</p>
 * @author B. Henry
 * @version 1.0
 */

public class QuickUsbBean extends JPanel {
  GridBagLayout gridBagLayout1 = new GridBagLayout();
  JLabel jLabel1 = new JLabel();
  JLabel jLabel2 = new JLabel();
  JComboBox ModuleComboBox = new JComboBox();
  JTextField DescriptionTextField = new JTextField();
  JLabel jLabel3 = new JLabel();
  JTextField DescriptionTextField1 = new JTextField();

  public QuickUsbBean() {
    try {
      jbInit();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    jLabel1.setText("Module");
    this.setLayout(gridBagLayout1);
    jLabel2.setToolTipText("");
    jLabel2.setText("Description");
    DescriptionTextField.setEditable(false);
    DescriptionTextField.setText("");
    jLabel3.setText("Serial");
    jLabel3.setToolTipText("");
    DescriptionTextField1.setText("");
    DescriptionTextField1.setEditable(false);
    this.setOpaque(false);
    this.add(jLabel1,                                   new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 5, 0, 0), 40, 0));
    this.add(jLabel2,                            new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 5, 0, 0), 0, 0));
    this.add(ModuleComboBox,                                                              new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 240, 0));
    this.add(DescriptionTextField,                                      new GridBagConstraints(1, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 240, 0));
    this.add(jLabel3,                                               new GridBagConstraints(0, 2, 1, 2, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 5, 0, 0), 0, 0));
    this.add(DescriptionTextField1,                                 new GridBagConstraints(1, 2, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 240, 0));
  }

  public boolean QuickUsbFindModules(String[] moduleList) {
    return true;
  }
}
