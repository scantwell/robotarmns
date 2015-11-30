/// <reference path='../_app.ts' />
/**
 * Created by omnia on 11/16/15.
 */
var RobotArm;
(function (RobotArm) {
    var IndexController = (function () {
        function IndexController(mdSideNav, http, mdDialog, timeout, interval) {
            var _this = this;
            this.mdSideNav = mdSideNav;
            this.http = http;
            this.mdDialog = mdDialog;
            this.timeout = timeout;
            this.interval = interval;
            this.status = 'boot';
            // Setup Ping Service
            this.interval(function () {
                _this.http.get('/api/ping').then(function () {
                    _this.status = "connected";
                }, function () {
                    _this.status = "disconnected";
                });
            }, 5000);
        }
        IndexController.prototype.toggleSidePane = function () {
            this.mdSideNav('left')
                .toggle()
                .then(function () {
                //console.log("Toggled Left is Done");
            });
        };
        IndexController.prototype.moveForward = function () {
            /* WIP - Loading Overaly
            this.mdDialog.show({
                templateUrl: '/templates/loadingOverlay.html',
                parent: angular.element(document.body),
                clickOutsideToClose: false
            });
            this.timeout(() => {
                this.mdDialog.hide();
            }, 1000);
            */
            var _this = this;
            this.http.post('/api/moveforward', { value: 50 }).then(function (success) {
                _this.timeout(function () {
                    _this.mdDialog.hide();
                });
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.turnLeft = function () {
            var _this = this;
            this.http.post('/api/turnleft', { value: 50 }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.turnRight = function () {
            var _this = this;
            this.http.post('/api/turnright', { value: 50 }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.moveBackward = function () {
            var _this = this;
            this.http.post('/api/movebackward', { value: 50 }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.openClaw = function () {
            var _this = this;
            this.http.post('/api/openclaw', { value: 50 }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.closeClaw = function () {
            var _this = this;
            this.http.post('/api/closeclaw', { value: 50 }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.raiseArm = function () {
            var _this = this;
            this.http.post('/api/raisearm', { value: 50 }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.lowerArm = function () {
            var _this = this;
            this.http.post('/api/lowerarm', { value: 50 }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        //noinspection JSUnusedGlobalSymbols
        IndexController.$inject = ['$mdSidenav', '$http', '$mdDialog', '$timeout', '$interval'];
        return IndexController;
    })();
    RobotArm.IndexController = IndexController;
    angular.module('RobotArm').controller('IndexController', IndexController);
})(RobotArm || (RobotArm = {}));
//# sourceMappingURL=index.js.map