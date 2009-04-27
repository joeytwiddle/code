# onchange "*.jpp" do eval 'jpp -- ./powerbar.html.jpp > powerbar.html'
onchange "*.jpp" do sh ./build.sh
