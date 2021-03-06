require File.dirname(__FILE__) + '/spec_helper'

load_extension("globals")

describe "CApiGlobalSpecs" do
  before :each do
    @f = CApiGlobalSpecs.new
  end
  
  it "correctly gets global values" do
    @f.sb_gv_get("$BLAH").should == nil
    @f.sb_gv_get("$SAFE").should == 0
    @f.sb_gv_get("SAFE").should == 0 # rb_gv_get should change SAFE to $SAFE
  end

  it "correctly sets global values" do
    @f.sb_gv_get("$BLAH").should == nil
    @f.sb_gv_set("$BLAH", 10)
    @f.sb_gv_get("$BLAH").should == 10
  end

  it "rb_define_variable should define a new global variable" do
    @f.rb_define_variable("my_gvar", "ABC")
    $my_gvar.should == "ABC"
    $my_gvar = "XYZ"
    @f.sb_get_global_value.should == "XYZ"
  end

  it "rb_define_readonly_variable should define a new readonly global variable" do
    @f.rb_define_readonly_variable("ro_gvar", 15)
    $ro_gvar.should == 15
    lambda { $ro_gvar = 10 }.should raise_error(NameError)
  end

  it "rb_define_hooked_variable should define a C hooked global variable" do
    @f.rb_define_hooked_variable_2x("$hooked_gvar")
    $hooked_gvar = 2
    $hooked_gvar.should == 4
  end
end
