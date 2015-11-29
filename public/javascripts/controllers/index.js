/// <reference path='../_app.ts' />
/**
 * Created by omnia on 11/16/15.
 */
var RobotArm;
(function (RobotArm) {
    var indexController = (function () {
        function indexController(location, mdSideNav, log) {
            this.location = location;
            this.mdSideNav = mdSideNav;
            this.log = log;
            this.text = "Sample Text";
        }
        indexController.prototype.toggleSidePane = function () {
            this.mdSideNav('left')
                .toggle()
                .then(function () {
                this.log.debug("Toggled Left is Done");
            });
        };
        //noinspection JSUnusedGlobalSymbols
        indexController.$inject = ['$location', '$mdSidenav', '$log'];
        return indexController;
    })();
    RobotArm.indexController = indexController;
    angular.module('RobotArm').controller('IndexController', indexController);
})(RobotArm || (RobotArm = {}));
//# sourceMappingURL=index.js.map