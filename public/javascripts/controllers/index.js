/// <reference path='../_app.ts' />
/**
 * Created by omnia on 11/16/15.
 */
var RobotArm;
(function (RobotArm) {
    var IndexController = (function () {
        function IndexController(mdSideNav, http, mdDialog, timeout, interval) {
            this.mdSideNav = mdSideNav;
            this.http = http;
            this.mdDialog = mdDialog;
            this.timeout = timeout;
            this.interval = interval;
            this.status = 'boot';
        }
        IndexController.prototype.toggleSidePane = function () {
            this.mdSideNav('left')
                .toggle()
                .then(function () { });
        };
        IndexController.prototype.moveForward = function () {
            var _this = this;
            this.http.post('/api/move', {
                movedirection: 1,
                centimeters: 5
            }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.turnLeft = function () {
            var _this = this;
            this.http.post('/api/rotate', {
                rotatedirection: 3,
                degrees: 10
            }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.turnRight = function () {
            var _this = this;
            this.http.post('/api/rotate', {
                rotatedirection: 4,
                degrees: 10
            }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.moveBackward = function () {
            var _this = this;
            this.http.post('/api/move', {
                movedirection: 2,
                centimeters: 5
            }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.openClaw = function () {
            var _this = this;
            this.http.post('/api/claw', {
                centimeters: 5
            }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.closeClaw = function () {
            var _this = this;
            this.http.post('/api/claw', {
                centimeters: 3
            }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.raiseArm = function () {
            var _this = this;
            this.http.post('/api/arm', {
                centimeters: 15
            }).then(function (success) {
                console.log(success);
            }, function (error) {
                _this.status = 'error';
                console.log(error);
            });
        };
        IndexController.prototype.lowerArm = function () {
            var _this = this;
            this.http.post('/api/arm', {
                centimeters: 5
            }).then(function (success) {
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